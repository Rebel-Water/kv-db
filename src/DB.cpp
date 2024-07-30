#include "DB.hpp"
#include <shared_mutex>
#include <mutex>
#include <filesystem>
#include <sys/file.h>
#include <fcntl.h>
#include "BTree.hpp"
#include "Util.hpp"
#include "Code.hpp"
#include "LogRecord.hpp"
#include "Batch.hpp"
#include <algorithm>
#include <gtest/gtest.h>

const std::string mergeDirName = "Merge";
const std::vector<byte> mergeFinishKey = Util::ToByteVector("merge-finished");
const std::string filelockName = "file-lock";

// about lock... maybe we can merge all lock in Btree into DB to simplify lock's logical
// have let all lock only stay at db, Btree's parallel hold by db
DB::DB(const Options &option) : option(option), seqNo(0), isMerging(false), isClose(false)
{
    switch (option.Type)
    {
    case BTree_Index:
        this->index = std::make_unique<BTree>();
        break;
    default:
        throw std::runtime_error("No Indexer Type Fit");
    }
    this->checkOptions(option);
    if (!std::filesystem::exists(option.DirPath))
    {
        if (!std::filesystem::create_directories(option.DirPath))
            throw std::runtime_error("DB::Open create_directory error");
    }
    auto lockfile = option.DirPath + filelockName;

    int fd = open(lockfile.data(), O_RDWR | O_CREAT, 0666);
    this->fileLockFd = fd;
    if (flock(fd, LOCK_EX) == -1)
    {
        close(fd);
        throw std::runtime_error("DB::Open File Lock Occupied");
    }

    this->LoadMergeFiles();
    this->LoadDataFiles();
    this->LoadHintFiles();
    this->LoadIndexFromDataFiles();
}

DB::~DB()
{
    this->Sync();
    if (!isClose)
        this->Close();
}

std::string DB::getMergePath()
{

    std::filesystem::path dirPath = this->option.DirPath;
    // Clean the path and get the directory part
    std::filesystem::path dir = dirPath.lexically_normal().parent_path();
    // Get the base name of the path
    std::filesystem::path base = dirPath.filename();
    // Append the merge directory name to the base
    std::filesystem::path fullPath = std::filesystem::path(dir) / (base.string() + mergeDirName);
    return fullPath.string();
}

void DB::Merge()
{
    if (this->activeFile == nullptr)
        return;

    uint32 nonMergeFileId;
    std::vector<std::shared_ptr<DataFile>> mergeFiles;
    {
        std::unique_lock<std::shared_mutex> lock(this->RWMutex);

        if (this->isMerging)
            throw std::runtime_error("DB::Merge DB Is Merging");

        this->isMerging = true;

        this->activeFile->Sync();

        this->olderFiles[this->activeFile->FileId] = this->activeFile;
        this->SetActiveDataFile();

        nonMergeFileId = this->activeFile->FileId;

        for (auto &[_, file] : this->olderFiles)
            mergeFiles.push_back(file);

        std::sort(mergeFiles.begin(), mergeFiles.end(),
                  [](auto &i, auto &j)
                  {
                      return i->FileId < j->FileId;
                  });
    }

    auto mergePath = this->getMergePath();

    if (std::filesystem::exists(mergePath))
    {
        if (!std::filesystem::remove_all(mergePath))
            throw std::runtime_error("DB::Merge Memoving Directory");
    }

    if (!std::filesystem::create_directories(mergePath))
    {
        throw std::runtime_error("DB::Merge Creating Directory");
    }

    Options mergeOptions(mergePath);
    DB mergeDB(mergeOptions);

    auto hintFile = std::make_shared<DataFile>(mergePath, 0, DataFileType::HintFile);

    for (auto &dataFile : mergeFiles)
    {
        uint64 offset = 0;
        while (auto logRecord = ReadLogRecord(offset, dataFile))
        {
            int size = logRecord->Size;
            auto key = WriteBatch::parseLogRecordKey(logRecord->Key).first;
            auto pos = this->index->Get(key);
            if (pos && pos->Fid == dataFile->FileId && pos->Offset == offset) // valid data
            {                                                                 // otherwise this k-v relation is not updated
                logRecord->Key = WriteBatch::logRecordKeyWithSeq(key, NonTransactionSeqNo);
                auto mergePos = mergeDB.AppendLogRecord(std::move(logRecord));
                this->WriteHintRecord(hintFile, key, mergePos);
            }
            offset += size;
        }
    }

    hintFile->Sync();
    mergeDB.Sync();

    auto mergeFinishFile = std::make_unique<DataFile>(mergePath, 0, DataFileType::MergeFinishFile);

    auto mergeFinishRecord = std::make_unique<LogRecord>(mergeFinishKey, Util::ToByteVector(std::to_string(nonMergeFileId)));

    auto encodeRecord = Code::EncodeLogRecord(std::move(mergeFinishRecord));
    mergeFinishFile->Write(encodeRecord);
    mergeFinishFile->Sync();

    this->isMerging = false;
}

void DB::Fold(FoldFn fn)
{
    std::unique_ptr<Iterator_Interface> iterator;
    {
        std::shared_lock<std::shared_mutex> lock(this->RWMutex);
        iterator = this->index->Iter(false);
        for (iterator->Rewind(); iterator->Valid(); iterator->Next())
        {
            auto value = this->getValueByPosition(iterator->Value());
            if (!fn(iterator->Key(), value))
                break;
        }
    }
    iterator->Close();
}

std::vector<std::vector<byte>> DB::ListKey()
{
    std::unique_ptr<Iterator_Interface> iter;
    {
        std::shared_lock<std::shared_mutex> lock(this->RWMutex);
        iter = this->index->Iter(false);
    }
    auto keys = std::vector<std::vector<byte>>();
    for (iter->Rewind(); iter->Valid(); iter->Next())
        keys.emplace_back(iter->Key());
    iter->Close();
    return keys;
}

void DB::Close()
{
    if (!this->isClose)
    {
        if (flock(fileLockFd, LOCK_UN) == -1)
            throw std::runtime_error("DB::~Close File UnLock Wrong");
        close(fileLockFd);
    } else 
    {
        return ;
    }
    if (this->activeFile == nullptr) ; 
    else {
        std::unique_lock<std::shared_mutex> lock(this->RWMutex);
        this->activeFile->Close();
        for (auto &[_, file] : this->olderFiles)
            file->Close();
    }
    isClose = true;
}

void DB::Sync()
{
    if (this->activeFile == nullptr)
        return;
    std::unique_lock<std::shared_mutex> lock(this->RWMutex);
    this->activeFile->Sync();
}

// write-lock
void DB::Put(const std::vector<byte> &key, const std::vector<byte> &value)
{
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (key.size() == 0)
        throw std::runtime_error("DB::Put Empty Key");
    auto log_record = std::make_unique<LogRecord>(WriteBatch::logRecordKeyWithSeq(key, NonTransactionSeqNo), value, LogRecordNormal);
    auto pos = this->AppendLogRecord(std::move(log_record));
    this->index->Put(key, pos);
}

// read-lock
std::vector<byte> DB::Get(const std::vector<byte> &key)
{
    std::shared_lock<std::shared_mutex> lock(this->RWMutex); // read lock
    if (key.size() == 0)
        throw std::runtime_error("DB::Get Empty Key");
    auto logRecordPos = this->index->Get(key);
    if (logRecordPos == nullptr)
        throw std::runtime_error("DB::Get Key Not Found");
    return getValueByPosition(*logRecordPos);
}

// write-lock
void DB::Delete(const std::vector<byte> &key)
{
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (key.size() == 0)
        throw std::runtime_error("DB::Delete Key Empty");
    auto pos = this->index->Get(key);
    if (pos == nullptr)
        return;
    auto logRecord = std::make_unique<LogRecord>(WriteBatch::logRecordKeyWithSeq(key, NonTransactionSeqNo), LogRecordDeleted); // Delete LogRecord size is smaller
    this->AppendLogRecord(std::move(logRecord));                                                                               // update at disk level to appand a delete flag
    // write-lock so here delete don't need lock
    // write-lock
    if (this->index->Delete(key) == 0) // update at memory level to call btree.delete
        throw std::runtime_error("DB::Delete Index Update Fail");
}

// call this function should hold read-lock
std::unique_ptr<LogRecord> DB::ReadLogRecord(int64 offset, std::shared_ptr<DataFile> datafile)
{
    int filesize = datafile->IoManager->Size();
    int headerBytes = MaxLogRecordHeaderSize;
    if (offset + MaxLogRecordHeaderSize > filesize)
        headerBytes = filesize - offset;

    auto headerBuf = datafile->readNBytes(headerBytes, offset);
    auto header = Code::DecodeLogRecord(headerBuf);

    if (header == nullptr)
        return nullptr;
    if (header->crc == 0 && header->keySize == 0 && header->valueSize == 0)
        return nullptr;

    int64 keySize = header->keySize, valueSize = header->valueSize;

    int64 recordSize = header->headerSize + keySize + valueSize;

    std::vector<byte> key, value;
    if (keySize > 0 || valueSize > 0)
    {
        auto kvBuf = datafile->readNBytes(keySize + valueSize, header->headerSize + offset);
        key = std::vector<byte>(kvBuf.begin(), kvBuf.begin() + keySize);
        value = std::vector<byte>(kvBuf.begin() + keySize, kvBuf.end());
    }

    auto logRecord = std::make_unique<LogRecord>(key, value, header->recordType);
    logRecord->Size = recordSize;

    std::vector<byte> crcBuf(headerBuf.begin() + sizeof(header->crc), headerBuf.begin() + header->headerSize);
    // crc notice value size is 0
    if (header->crc != logRecord->getLogRecordCRC(crcBuf))
        throw std::runtime_error("DB::ReadLogRecord Crc fault");

    return std::move(logRecord);
}

// call this function should hold write-lock
// merge except
LogRecordPos DB::AppendLogRecord(std::unique_ptr<LogRecord> logRecord) // here appandlogrecord can be moved into datafile?
                                                                       // now i move appand and read all into db
{
    if (this->activeFile == nullptr) // when start db, activefile is null
        this->SetActiveDataFile();

    std::vector<byte> encodeRecord = Code::EncodeLogRecord(std::move(logRecord));
    int size = encodeRecord.size();
    if (this->activeFile->WriteOff + size > this->option.DataFileSize)
    {
        this->activeFile->Sync();
        this->olderFiles[this->activeFile->FileId] = this->activeFile;
        this->SetActiveDataFile();
    }

    auto writeOff = this->activeFile->WriteOff;
    this->activeFile->Write(encodeRecord);

    if (this->option.SyncWrite)
        this->activeFile->Sync();

    return LogRecordPos(this->activeFile->FileId, writeOff);
}

void DB::WriteHintRecord(std::shared_ptr<DataFile> datafile, const std::vector<byte> &key, LogRecordPos pos)
{
    auto record = std::make_unique<LogRecord>(key, Code::EncodeLogRecordPos(pos));
    auto encodeRecord = Code::EncodeLogRecord(std::move(record));
    datafile->Write(encodeRecord);
}

std::unique_ptr<WriteBatch> DB::NewWriteBatch(WriteBatchOptions option)
{
    return std::make_unique<WriteBatch>(option, this);
}

std::unique_ptr<Iterator> DB::NewIterator(IteratorOptions option)
{
    auto indexIter = this->index->Iter(option.Reverse);
    return std::make_unique<Iterator>(this, std::move(indexIter), option);
}

std::vector<byte> DB::getValueByPosition(const LogRecordPos &pos)
{
    std::shared_lock<std::shared_mutex> lock(this->RWMutex);
    std::shared_ptr<DataFile> datafile;
    if (this->activeFile->FileId == pos.Fid)
        datafile = this->activeFile;
    else if (this->olderFiles.find(pos.Fid) != this->olderFiles.end())
        datafile = this->olderFiles[pos.Fid]; // note: we also should know db's member data lock logic

    auto logRecord = std::move(this->ReadLogRecord(pos.Offset, datafile)); // can overfit

    if (logRecord->Type == LogRecordDeleted)
        throw std::runtime_error("DB::Get Error Key Not Found");

    return logRecord->Value;
}

// callee by appendlogrecord, have write-lock
// before callee, olderfile has been set
void DB::SetActiveDataFile()
{
    uint32 initialFieldId = 0;
    if (this->activeFile != nullptr) // start from 0 because db initial activeFile is nullptr
    {
        initialFieldId = this->activeFile->FileId + 1;
    }
    auto dataFile = std::make_shared<DataFile>(this->option.DirPath, initialFieldId);
    if (dataFile == nullptr)
    {
        throw std::runtime_error("DB::SetActiveDataFile no such datafile");
    }
    this->activeFile = dataFile;
}

void DB::checkOptions(const Options &option)
{
    if (option.DirPath == "")
        throw std::runtime_error("DB::checkOptions Dirpath Empty");
    if (option.DataFileSize <= 0)
        throw std::runtime_error("DB::checkOptions DataFileSize Too Low");
}

void DB::LoadDataFiles()
{
    std::vector<int> fileIds;
    for (const auto &entry : std::filesystem::directory_iterator(this->option.DirPath))
    {
        if (entry.is_regular_file())
        {
            const std::string fileName = entry.path().filename().string();
            if (fileName.size() >= DataFileNameSuffix.size() &&
                fileName.compare(fileName.size() - DataFileNameSuffix.size(), DataFileNameSuffix.size(), DataFileNameSuffix) == 0)
            {
                // Split the file name
                std::istringstream ss(fileName);
                std::string token;
                std::getline(ss, token, '.');  // Get the first part before the dot
                int fileId = std::stoi(token); // use number as name
                fileIds.push_back(fileId);
            }
        }
    }
    sort(fileIds.begin(), fileIds.end());
    this->fileIds = fileIds;
    int n = fileIds.size();
    for (int i = 0; i < n; i++)
    {
        auto dataFile = std::make_shared<DataFile>(this->option.DirPath, fileIds[i]);
        if (i == n - 1)
            this->activeFile = dataFile; // only the biggest number file can be read, the newest
        else
            this->olderFiles[fileIds[i]] = dataFile;
    }
}

void DB::LoadIndexFromDataFiles() // now kv record all in disk, read from it and load it into btree-memory
{
    if (this->fileIds.size() == 0) // fileIds(vector) got in loadDataFiles, this vector maintain all old datafile and active datafile in DB
        return;

    bool hasMerge = false;
    uint32 nonMergeField = 0;
    auto mergeFinishFileName = std::filesystem::path(this->option.DirPath) / MergeFinishedFileName;
    if (std::filesystem::exists(mergeFinishFileName))
    {
        nonMergeField = this->GetNonMergeFileId(this->option.DirPath);
        hasMerge = true;
    }

    auto updateIndex = [&](const std::vector<byte> &key, LogRecordType type, LogRecordPos pos)
    {
        if (type == LogRecordDeleted) // delete is a record to appand into disk but in memory we do delete
        {
            if (this->index->Delete(key) == 0)
                throw std::runtime_error("DB::LoadIndexFromDataFiles Update Delete Index Fail");
        }
        else if (type == LogRecordNormal) // modify!
        {
            if (this->index->Put(key, pos) == 0)
                throw std::runtime_error("DB::LoadIndexFromDataFiles Update Put Index Fail");
        }
    };

    std::unordered_map<uint64, std::vector<std::unique_ptr<TransactionRecord>>> transactionRecords;
    int currentSeqno = NonTransactionSeqNo;

    int n = fileIds.size();
    for (int i = 0; i < n; i++)
    {
        uint32 fileId = fileIds[i];
        if (hasMerge && fileId < nonMergeField)
            continue;
        std::shared_ptr<DataFile> datafile;
        if (fileId == this->activeFile->FileId)
            datafile = this->activeFile;
        else
            datafile = this->olderFiles[fileId];
        int64 offset = 0;                                              // from loading every datafile, offset should begin at 0
        while (auto logRecord = this->ReadLogRecord(offset, datafile)) // find fileId.data datafile to load in btree
        {                                                              // and read target.data in offset(int offset = 0)
            int size = logRecord->Size;
            auto logRecordPos = LogRecordPos(fileId, offset);
            auto pair = WriteBatch::parseLogRecordKey(logRecord->Key);
            if (pair.second == NonTransactionSeqNo)
            {
                updateIndex(pair.first, logRecord->Type, logRecordPos);
            }
            else
            {
                if (logRecord->Type == LogRecordTxnFinsihed)
                {
                    for (auto &txnRecord : transactionRecords[pair.second])
                        updateIndex(txnRecord->Record->Key, txnRecord->Record->Type, txnRecord->Pos);
                    transactionRecords.erase(pair.second);
                }
                else
                {
                    logRecord->Key = pair.first;
                    transactionRecords[pair.second].emplace_back(std::make_unique<TransactionRecord>(std::move(logRecord), logRecordPos));
                }
            }
            if (pair.second > currentSeqno)
                currentSeqno = pair.second;

            offset += size; // after readed, offset increase and point at next logRecord start
        }
        if (i == n - 1) // we write the newest(biggest) datafile, others can only be read can't be wrote
            this->activeFile->WriteOff = offset;
    }
    this->seqNo = currentSeqno;
}

void DB::LoadMergeFiles()
{
    std::string mergePath = getMergePath();

    if (!std::filesystem::exists(mergePath))
        return;

    std::vector<std::filesystem::directory_entry> dirEntries;

    for (const auto &entry : std::filesystem::directory_iterator(mergePath))
        dirEntries.push_back(entry);

    bool mergeFinished = false;
    std::vector<std::string> mergeFileNames;

    for (const auto &entry : dirEntries)
    {
        auto filename = entry.path().filename();
        if (filename == MergeFinishedFileName)
            mergeFinished = true;
        if (filename != MergeFinishedFileName && filename != HintFileName)
        {
            mergeFileNames.push_back(filename);
            GTEST_LOG_(INFO) << filename;
        }
    }

    GTEST_LOG_(INFO) << "mergeFinished: " << mergeFinished;
    if (!mergeFinished)
        return;

    uint32 nonMergeFileId = this->GetNonMergeFileId(mergePath);
    GTEST_LOG_(INFO) << "nonMergeFileId: " << nonMergeFileId;
    uint32 fileId = 0;
    for (; fileId < nonMergeFileId; fileId++)
    {
        auto fileName = DataFile::GetDataFileName(this->option.DirPath, fileId);
        GTEST_LOG_(INFO) << "fileId: " << fileId << " and fileName: " << fileName << std::endl;
        if (std::filesystem::exists(fileName))
            std::filesystem::remove(fileName);
    }

    for (auto &fileName : mergeFileNames)
    {
        auto srcPath = std::filesystem::path(mergePath) / fileName;
        auto destPath = std::filesystem::path(this->option.DirPath) / fileName;
        std::filesystem::rename(srcPath, destPath);
    }

    std::filesystem::remove_all(mergePath);
}

void DB::LoadHintFiles()
{
    auto hintFileName = std::filesystem::path(this->option.DirPath) / HintFileName;
    GTEST_LOG_(INFO) << hintFileName << std::endl;
    if (!std::filesystem::exists(hintFileName))
        return;
    auto hintFile = std::make_shared<DataFile>(this->option.DirPath, DataFileType::HintFile);
    int64 offset = 0;
    while (auto logRecord = ReadLogRecord(offset, hintFile))
    {
        auto pos = Code::DecodeLogRecordPos(logRecord->Value);
        this->index->Put(logRecord->Key, pos);
        offset += logRecord->Size;
    }
}

uint32 DB::GetNonMergeFileId(const std::string &dirPath)
{
    auto mergeFinishFile = std::make_shared<DataFile>(dirPath, 0, DataFileType::MergeFinishFile);
    GTEST_LOG_(INFO) << "mergeFinishFile: " << std::filesystem::path(dirPath) / MergeFinishedFileName;
    auto record = DB::ReadLogRecord(0, mergeFinishFile);
    int nonMergeFileId = std::stoi(Util::ToString(record->Value));
    return static_cast<uint32>(nonMergeFileId);
}
