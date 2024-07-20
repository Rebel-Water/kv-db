#include "DB.hpp"
#include <shared_mutex>
#include <mutex>
#include <filesystem>
#include "BTree.hpp"
#include "Code.hpp"
#include "LogRecord.hpp"
#include <algorithm>

DB::DB(Options option) : option(option)
{
    switch (option.Type)
    {
    case BTree_Index:
        this->index = std::make_unique<BTree>();
        break;
    default:
        throw "No Indexer Type Fit";
    }
}

std::unique_ptr<DB> DB::Open(Options option)
{
    this->checkOptions(option);
    if (std::filesystem::exists(option.DirPath))
        if (!std::filesystem::create_directories(option.DirPath))
            throw "DB::Open create_directory error";
    
    auto db = std::make_unique<DB>(option);
    db->LoadDataFiles();
    db->LoadIndexFromDataFiles();
    return std::move(db);
}

void DB::Put(std::vector<byte> &key, std::vector<byte> &value)
{
    if (key.size() == 0)
        throw "DB::Put Empty Key";
    auto log_record = std::make_unique<LogRecord>(key, value, LogRecordNormal);
    auto pos = this->AppendLogRecord(std::move(log_record));
    this->index->Put(key, std::move(pos));
}

std::vector<byte> DB::Get(const std::vector<byte> &key)
{
    if (key.size() == 0)
        throw "DB::Get Empty Key";
    std::shared_lock<std::shared_mutex> lock(this->RWMutex);
    auto logRecordPos = this->index->Get(key);
    if (logRecordPos == nullptr)
        throw "DB::Get Key Not Found";

    std::unique_ptr<LogRecord> logRecord;
    std::shared_ptr<DataFile> datafile;
    if (this->activeFile->FileId == logRecordPos->Fid)
        datafile = this->activeFile;
    else if (this->olderFiles.find(logRecordPos->Fid) != this->olderFiles.end())
        datafile = this->olderFiles[logRecordPos->Fid];

    logRecord = std::move(datafile->ReadLogRecord(logRecordPos->Offset)); // can overfit

    if (logRecord->Type == LogRecordDeleted)
        throw "DB::Get Error Key Not Found";

    return logRecord->Value;
}

std::unique_ptr<LogRecordPos> DB::AppendLogRecord(std::unique_ptr<LogRecord> logRecord)
{
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    if (this->activeFile == nullptr)
    {
        this->SetActiveDataFile();
    }
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
    {
        this->activeFile->Sync();
    }
    return std::make_unique<LogRecordPos>(this->activeFile->FileId, writeOff);
}

void DB::SetActiveDataFile()
{
    uint32 initialFieldId = 0;
    if (this->activeFile != nullptr)
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

void DB::checkOptions(Options option)
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
                std::getline(ss, token, '.'); // Get the first part before the dot
                int fileId = std::stoi(token); // use number as name
                fileIds.push_back(fileId);
            }
        }
    }
    sort(fileIds.begin(), fileIds.end());
    this->fileIds = fileIds;
    int n = fileIds.size();
    for(int i = 0; i < n; i++) {
        auto dataFile = std::make_shared<DataFile>(this->option.DirPath, fileIds[i]);
        if(i == n - 1)
            this->activeFile = dataFile;
        else 
            this->olderFiles[fileIds[i]] = dataFile;
    }
    return ;
}

void DB::LoadIndexFromDataFiles() {
    if(this->fileIds.size() == 0)
        return ;
    
    int n = fileIds.size();
    for(int i = 0; i < n; i++) {
        uint32 fileId = fileIds[i];
        std::shared_ptr<DataFile> datafile;
        if(fileId == this->activeFile->FileId)
            datafile = this->activeFile;
        else
            datafile = this->olderFiles[fileId];
        int64 offset = 0;
        while(auto logRecord = datafile->ReadLogRecord(offset)) {
            auto logRecordPos = std::make_unique<LogRecordPos>(fileId, offset);
            if(logRecord->Type == LogRecordDeleted)
                this->index->Delete(logRecord->Key);
            else
                this->index->Put(logRecord->Key, std::move(logRecordPos));
            offset += logRecord->Size;
        }
        if(i == n - 1)
            this->activeFile->WriteOff = offset;
    }
}