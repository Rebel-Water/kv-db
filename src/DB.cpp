#include "DB.hpp"
#include "lib/file.hpp"
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
#include "Mmap.hpp"
#include <algorithm>
#include <gtest/gtest.h>

const std::string mergeDirName = "Merge";
const std::vector<byte> mergeFinishKey = Util::ToByteVector("merge-finished");
const std::string filelockName = "filelock";

// about lock... maybe we can merge all lock in Btree into DB to simplify lock's logical
// have let all lock only stay at db, Btree's parallel hold by db
DB::DB(const Options &option) : option(option), seqNo(0), isMerging(false), isClose(false)
{
    switch (option.Type)
    {
    case BTree_Index:
        index = std::make_unique<BTree>();
        break;
    default:
        throw std::runtime_error("No Indexer Type Fit");
    }
    CheckOptions(option);
    if (!std::filesystem::exists(option.DirPath))
    {
        if (!std::filesystem::create_directories(option.DirPath))
            throw std::runtime_error("DB::Open create_directory error");
    }
    auto lockfile = std::filesystem::path(option.DirPath) / filelockName;

    int fd = open(lockfile.c_str(), O_RDWR | O_CREAT, 0666);
    fileLockFd = fd;
    if (flock(fd, LOCK_EX | LOCK_NB) == -1)
    {
        close(fd);
        throw std::runtime_error("DB::Open File Lock Occupied");
    }

    reclaimableSize = 0;

    LoadMergeFiles();
    LoadDataFiles();
    LoadHintFiles();
    LoadIndexFromDataFiles();
}

DB::~DB()
{
    Sync();
    if (!isClose)
        Close();
}

std::string DB::getMergePath()
{

    std::filesystem::path dirPath = option.DirPath;
    // Clean the path and get the directory part
    std::filesystem::path dir = dirPath.lexically_normal().parent_path();
    // Get the base name of the path
    std::filesystem::path base = dirPath.filename();
    // Append the merge directory name to the base
    std::filesystem::path fullPath = std::filesystem::path(dir) / (base.string() + mergeDirName);
    return fullPath.string();
}

void DB::BackUp(const std::string& dest)
{
    std::shared_lock<std::shared_mutex> lock(RWMutex);
    File::CopyDir(std::filesystem::path(option.DirPath), dest, {filelockName});
}

Stat DB::Statement()
{
    std::shared_lock<std::shared_mutex> lock(RWMutex);
    int dataFiles = olderFiles.size();
    if (activeFile)
        dataFiles++;
    return Stat(index->Size(), dataFiles, reclaimableSize, File::DirSize(option.DirPath));
}

void DB::Merge()
{
    if (activeFile == nullptr)
        return;

    uint32 nonMergeFileId;
    std::vector<std::shared_ptr<DataFile>> mergeFiles;
    {
        std::unique_lock<std::shared_mutex> lock(RWMutex);

        if (isMerging)
            throw std::runtime_error("DB::Merge DB Is Merging");

        if (option.isOpenMergeDetect)
        {
            auto totalSize = File::DirSize(option.DirPath);
            if (reclaimableSize * 1.0 / totalSize < this->option.DataFileMergeRatio)
                return;
            auto availDiskSize = File::AvailableDiskSize();
            if (totalSize - reclaimableSize >= availDiskSize)
                throw std::runtime_error("DB::Merge No Enough Space For Merge");
        }

        isMerging = true;
        activeFile->Sync();
        olderFiles[this->activeFile->FileId] = this->activeFile;
        SetActiveDataFile();

        nonMergeFileId = activeFile->FileId;

        for (auto &[_, file] : olderFiles)
            mergeFiles.push_back(file);

        std::sort(mergeFiles.begin(), mergeFiles.end(),
                  [](auto &i, auto &j)
                  {
                      return i->FileId < j->FileId;
                  });
    }

    auto mergePath = getMergePath();

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
            auto pos = index->Get(key);
            if (!pos.isEmpty && pos.Fid == dataFile->FileId && pos.Offset == offset) // valid data
            {                                                                        // otherwise this k-v relation is not updated
                logRecord->Key = WriteBatch::logRecordKeyWithSeq(key, NonTransactionSeqNo);
                auto mergePos = mergeDB.AppendLogRecord(std::move(logRecord));
                WriteHintRecord(hintFile, key, mergePos);
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

    isMerging = false;
}

void DB::Fold(FoldFn fn)
{
    std::unique_ptr<Iterator_Interface> iterator;
    {
        std::shared_lock<std::shared_mutex> lock(RWMutex);
        iterator = index->Iter(false);
        for (iterator->Rewind(); iterator->Valid(); iterator->Next())
        {
            auto value = GetValueByPosition(iterator->Value());
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
        std::shared_lock<std::shared_mutex> lock(RWMutex);
        iter = index->Iter(false);
    }
    auto keys = std::vector<std::vector<byte>>();
    for (iter->Rewind(); iter->Valid(); iter->Next())
        keys.emplace_back(iter->Key());
    iter->Close();
    return keys;
}

void DB::Close()
{
    if (!isClose)
    {
        if (flock(fileLockFd, LOCK_UN) == -1)
            throw std::runtime_error("DB::~Close File UnLock Wrong");
        close(fileLockFd);
    }
    else
    {
        return;
    }
    if (activeFile == nullptr)
        ;
    else
    {
        std::unique_lock<std::shared_mutex> lock(RWMutex);
        activeFile->Close();
        for (auto &[_, file] : olderFiles)
            file->Close();
    }
    isClose = true;
}

void DB::Sync()
{
    if (activeFile == nullptr)
        return;
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    activeFile->Sync();
}

// write-lock
void DB::Put(const std::vector<byte> &key, const std::vector<byte> &value)
{
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (key.size() == 0)
        throw std::runtime_error("DB::Put Empty Key");
    auto log_record = std::make_unique<LogRecord>(WriteBatch::logRecordKeyWithSeq(key, NonTransactionSeqNo), value, LogRecordNormal);
    auto pos = AppendLogRecord(std::move(log_record));
    auto ret = index->Put(key, pos);
    if (!ret.isEmpty)
        reclaimableSize += ret.Size;
}

// read-lock
std::vector<byte> DB::Get(const std::vector<byte> &key)
{
    std::shared_lock<std::shared_mutex> lock(RWMutex); // read lock
    if (key.size() == 0)
        throw std::runtime_error("DB::Get Empty Key");
    auto logRecordPos = index->Get(key);
    if (logRecordPos.isEmpty)
        throw std::runtime_error("DB::Get Key Not Found");
    return GetValueByPosition(logRecordPos);
}

// write-lock
void DB::Delete(const std::vector<byte> &key)
{
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (key.size() == 0)
        throw std::runtime_error("DB::Delete Key Empty");
    auto pos = index->Get(key);
    if (pos.isEmpty)
        return;
    auto logRecord = std::make_unique<LogRecord>(WriteBatch::logRecordKeyWithSeq(key, NonTransactionSeqNo), LogRecordDeleted); // Delete LogRecord size is smaller
    AppendLogRecord(std::move(logRecord));                                                                               // update at disk level to appand a delete flag
    // write-lock so here delete don't need lock
    // write-lock
    auto ret = index->Delete(key);
    if (ret.isEmpty) // update at memory level to call btree.delete
        throw std::runtime_error("DB::Delete Index Update Fail");
    else
        reclaimableSize += ret.Size;
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
    if (header->crc != logRecord->GetLogRecordCRC(crcBuf))
        throw std::runtime_error("DB::ReadLogRecord Crc fault");

    return std::move(logRecord);
}

// call this function should hold write-lock
// merge except
LogRecordPos DB::AppendLogRecord(std::unique_ptr<LogRecord> logRecord) // here appandlogrecord can be moved into datafile?
                                                                       // now i move appand and read all into db
{
    if (activeFile == nullptr) // when start db, activefile is null
        SetActiveDataFile();

    std::vector<byte> encodeRecord = Code::EncodeLogRecord(std::move(logRecord));
    int size = encodeRecord.size();
    if (activeFile->WriteOff + size > this->option.DataFileSize)
    {
        activeFile->Sync();
        olderFiles[this->activeFile->FileId] = this->activeFile;
        SetActiveDataFile();
    }

    auto writeOff = activeFile->WriteOff;
    activeFile->Write(encodeRecord);

    bytesWrite += size;

    if (option.SyncWrite && this->option.BytesPerSync && this->bytesWrite >= this->option.BytesPerSync)
    {
        bytesWrite = 0;
        activeFile->Sync();
    }

    return LogRecordPos(activeFile->FileId, writeOff, size);
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
    auto indexIter = index->Iter(option.Reverse);
    return std::make_unique<Iterator>(this, std::move(indexIter), option);
}

std::vector<byte> DB::GetValueByPosition(const LogRecordPos &pos)
{
    std::shared_lock<std::shared_mutex> lock(RWMutex);
    std::shared_ptr<DataFile> datafile;
    if (activeFile->FileId == pos.Fid)
        datafile = activeFile;
    else if (olderFiles.find(pos.Fid) != this->olderFiles.end())
        datafile = olderFiles[pos.Fid]; // note: we also should know db's member data lock logic

    auto logRecord = std::move(ReadLogRecord(pos.Offset, datafile)); // can overfit

    if (logRecord->Type == LogRecordDeleted)
        throw std::runtime_error("DB::Get Error Key Not Found");

    return logRecord->Value;
}

// callee by appendlogrecord, have write-lock
// before callee, olderfile has been set
void DB::SetActiveDataFile()
{
    uint32 initialFieldId = 0;
    if (activeFile != nullptr) // start from 0 because db initial activeFile is nullptr
    {
        initialFieldId = activeFile->FileId + 1;
    }
    auto dataFile = std::make_shared<DataFile>(option.DirPath, initialFieldId);
    if (dataFile == nullptr)
    {
        throw std::runtime_error("DB::SetActiveDataFile no such datafile");
    }
    activeFile = dataFile;
}

void DB::CheckOptions(const Options &option)
{
    if (option.DirPath == "")
        throw std::runtime_error("DB::checkOptions Dirpath Empty");
    if (option.DataFileSize <= 0)
        throw std::runtime_error("DB::checkOptions DataFileSize Too Low");
    if (option.DataFileMergeRatio < 0 || option.DataFileMergeRatio > 1)
        throw std::runtime_error("DB::checkOptions DataFileMergeRatio Is Not In Range");
}

void DB::LoadDataFiles()
{
    std::vector<int> fileIds;
    for (const auto &entry : std::filesystem::directory_iterator(option.DirPath))
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
    fileIds = fileIds;
    int n = fileIds.size();
    for (int i = 0; i < n; i++)
    {
        if (i == n - 1)
        {
            auto dataFile = std::make_shared<DataFile>(option.DirPath, fileIds[i] /*, DataFileType::DataNormalFile, IOType::MmapType*/);
            activeFile = dataFile; // only the biggest number file can be read, the newest
        }
        else
        {
            auto dataFile = std::make_shared<DataFile>(option.DirPath, fileIds[i], DataFileType::DataNormalFile, IOType::MmapType);
            olderFiles[fileIds[i]] = dataFile;
        }
    }
}

void DB::LoadIndexFromDataFiles() // now kv record all in disk, read from it and load it into btree-memory
{
    if (fileIds.size() == 0) // fileIds(vector) got in loadDataFiles, this vector maintain all old datafile and active datafile in DB
        return;

    bool hasMerge = false;
    uint32 nonMergeField = 0;
    auto mergeFinishFileName = std::filesystem::path(option.DirPath) / MergeFinishedFileName;
    if (std::filesystem::exists(mergeFinishFileName))
    {
        nonMergeField = GetNonMergeFileId(this->option.DirPath);
        hasMerge = true;
    }

    auto updateIndex = [&](const std::vector<byte> &key, LogRecordType type, LogRecordPos pos)
    {
        if (type == LogRecordDeleted) // delete is a record to appand into disk but in memory we do delete
        {
            if (index->Delete(key).isEmpty)
                throw std::runtime_error("DB::LoadIndexFromDataFiles Update Delete Index Fail");
        }
        else if (type == LogRecordNormal)
        {
            index->Put(key, pos);
            // if (!index->Put(key, pos).isEmpty)
            //     throw std::runtime_error("DB::LoadIndexFromDataFiles Update Put Index Fail");
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
        if (fileId == activeFile->FileId)
            datafile = activeFile;
        else
            datafile = olderFiles[fileId];
        int64 offset = 0;                                              // from loading every datafile, offset should begin at 0
        while (auto logRecord = ReadLogRecord(offset, datafile)) // find fileId.data datafile to load in btree
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
            activeFile->WriteOff = offset;
    }
    seqNo = currentSeqno;
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
        if (filename != MergeFinishedFileName && filename != HintFileName && filename != filelockName)
        {
            mergeFileNames.push_back(filename);
        }
    }

    if (!mergeFinished)
        return;

    uint32 nonMergeFileId = GetNonMergeFileId(mergePath);
    uint32 fileId = 0;
    for (; fileId < nonMergeFileId; fileId++)
    {
        auto fileName = DataFile::GetDataFileName(option.DirPath, fileId);
        if (std::filesystem::exists(fileName))
            std::filesystem::remove(fileName);
    }

    for (auto &fileName : mergeFileNames)
    {
        auto srcPath = std::filesystem::path(mergePath) / fileName;
        auto destPath = std::filesystem::path(option.DirPath) / fileName;
        std::filesystem::rename(srcPath, destPath);
    }

    std::filesystem::remove_all(mergePath);
}

void DB::LoadHintFiles()
{
    auto hintFileName = std::filesystem::path(option.DirPath) / HintFileName;
    if (!std::filesystem::exists(hintFileName))
        return;
    auto hintFile = std::make_shared<DataFile>(option.DirPath, 0, DataFileType::HintFile, IOType::MmapType);
    int64 offset = 0;
    while (auto logRecord = ReadLogRecord(offset, hintFile))
    {
        auto pos = Code::DecodeLogRecordPos(logRecord->Value);
        index->Put(logRecord->Key, pos);
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
