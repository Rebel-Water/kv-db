#include "DB.hpp"
#include <shared_mutex>
#include <mutex>
#include <filesystem>
#include "BTree.hpp"
#include "Code.hpp"
#include "LogRecord.hpp"
#include <algorithm>

// about lock... maybe we can merge all lock in Btree into DB to simplify lock's logical
// have let all lock only stay at db, Btree's parallel hold by db
DB::DB(const Options &option) : option(option)
{
    switch (option.Type)
    {
    case BTree_Index:
        this->index = std::make_unique<BTree>();
        break;
    default:
        throw "No Indexer Type Fit";
    }
    this->checkOptions(option);
    if (!std::filesystem::exists(option.DirPath))
    {
        if (!std::filesystem::create_directories(option.DirPath))
            throw "DB::Open create_directory error";
    }

    this->LoadDataFiles();
    this->LoadIndexFromDataFiles();
}

// write-lock
void DB::Put(const std::vector<byte> &key, const std::vector<byte> &value)
{
    // std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (key.size() == 0)
        throw "DB::Put Empty Key";
    auto log_record = std::make_unique<LogRecord>(key, value, LogRecordNormal);
    auto pos = this->AppendLogRecord(std::move(log_record));
    this->index->Put(key, std::move(pos));
}

// read-lock
std::vector<byte> DB::Get(const std::vector<byte> &key)
{
    // std::shared_lock<std::shared_mutex> lock(this->RWMutex); // read lock
    if (key.size() == 0)
        throw "DB::Get Empty Key";
    auto logRecordPos = this->index->Get(key);
    if (logRecordPos == nullptr)
        throw "DB::Get Key Not Found";

    std::unique_ptr<LogRecord> logRecord;
    std::shared_ptr<DataFile> datafile;
    if (this->activeFile->FileId == logRecordPos->Fid)
        datafile = this->activeFile;
    else if (this->olderFiles.find(logRecordPos->Fid) != this->olderFiles.end())
        datafile = this->olderFiles[logRecordPos->Fid]; // note: we also should know db's member data lock logic

    logRecord = std::move(this->ReadLogRecord(logRecordPos->Offset, datafile)); // can overfit

    if (logRecord->Type == LogRecordDeleted)
        throw "DB::Get Error Key Not Found";

    return logRecord->Value;
}

// write-lock
void DB::Delete(const std::vector<byte> &key)
{
    // std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (key.size() == 0)
        throw "DB::Delete Key Empty";

    if(auto pos = this->index->Get(key) != nullptr)
        return;
    auto logRecord = std::make_unique<LogRecord>(key, LogRecordDeleted); // Delete LogRecord size is smaller
    this->AppendLogRecord(std::move(logRecord)); // update at disk level to appand a delete flag
    // write-lock so here delete don't need lock
    // write-lock
    if (this->index->Delete(key) == 0) // update at memory level to call btree.delete
        throw "DB::Delete Index Update Fail";
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
        auto kvBuf = datafile->readNBytes(keySize + valueSize, header->headerSize + offset); // wrong!
        key = std::vector<byte>(kvBuf.begin(), kvBuf.begin() + keySize);
        value = std::vector<byte>(kvBuf.begin() + keySize, kvBuf.end());
    }

    auto logRecord = std::make_unique<LogRecord>(key, value, header->recordType);
    logRecord->Size = recordSize;

    std::vector<byte> crcBuf(headerBuf.begin() + sizeof(header->crc), headerBuf.begin() + header->headerSize);
    if (header->crc != logRecord->getLogRecordCRC(crcBuf))
        throw "DB::ReadLogRecord Crc fault";

    return std::move(logRecord);
}

// call this function should hold write-lock
std::unique_ptr<LogRecordPos> DB::AppendLogRecord(std::unique_ptr<LogRecord> logRecord) // here appandlogrecord can be moved into datafile?
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

    return std::make_unique<LogRecordPos>(this->activeFile->FileId, writeOff);
}
// callee by appendlogrecord, have write-lock
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
        throw "DB::SetActiveDataFile no such datafile";
    }
    this->activeFile = dataFile;
}

void DB::checkOptions(const Options &option)
{
    if (option.DirPath == "")
        throw "DB::checkOptions Dirpath Empty";
    if (option.DataFileSize <= 0)
        throw "DB::checkOptions DataFileSize Too Low";
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
                // printf("%d\n", fileId);
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

    int n = fileIds.size();
    for (int i = 0; i < n; i++)
    {
        uint32 fileId = fileIds[i];
        std::shared_ptr<DataFile> datafile;
        if (fileId == this->activeFile->FileId)
            datafile = this->activeFile;
        else
            datafile = this->olderFiles[fileId];
        int64 offset = 0;                                              // from loading every datafile, offset should begin at 0
        while (auto logRecord = this->ReadLogRecord(offset, datafile)) // find fileId.data datafile to load in btree
        {                                                              // and read target.data in offset(int offset = 0)
            auto logRecordPos = std::make_unique<LogRecordPos>(fileId, offset);
            if (logRecord->Type == LogRecordDeleted) // delete is a record to appand into disk but in memory we do delete
            {
                if (this->index->Delete(logRecord->Key) == 0)
                    throw "DB::LoadIndexFromDataFiles Update Delete Index Fail";
            }
            else
            {
                if (this->index->Put(logRecord->Key, std::move(logRecordPos)) == 0) {
                    throw "DB::LoadIndexFromDataFiles Update Put Index Fail";
                }
            }
            offset += logRecord->Size; // after readed, offset increase and point at next logRecord start
        }
        if (i == n - 1) // we write the newest(biggest) datafile, others can only be read can't be wrote
            this->activeFile->WriteOff = offset;
    }
}
