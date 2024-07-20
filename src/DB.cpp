#include "DB.hpp"
#include <shared_mutex>
#include <mutex>
#include "Code.hpp"
#include "LogRecord.hpp"

void DB::Put(std::vector<byte>& key, std::vector<byte>& value) {
    if(key.size() == 0)
        throw "DB::Put Empty Key";
    auto log_record = std::make_unique<LogRecord>(key, value, LogRecordNormal);
    auto pos = this->AppendLogRecord(std::move(log_record));
    this->index->Put(key, std::move(pos));
}

std::vector<byte> DB::Get(const std::vector<byte>& key) {
    if(key.size() == 0)
        throw "DB::Get Empty Key";
    auto logRecordPos = this->index->Get(key);
    if(logRecordPos == nullptr)
        throw "DB::Get Key Not Found";

    std::unique_ptr<LogRecord> logRecord;
    if(this->activeFile->FileId == logRecordPos->Fid) {
        logRecord = std::move(this->activeFile->ReadLogRecord(logRecordPos->Offset));
    } else if(this->olderFiles.find(logRecordPos->Fid) != this->olderFiles.end()) {
        logRecord = std::move(this->olderFiles[logRecordPos->Fid]->ReadLogRecord(logRecordPos->Offset));
    }

    if(logRecord->Type == LogRecordDeleted)
        throw "DB::Get Error Key Not Found";
    
    return logRecord->Value;
}

std::unique_ptr<LogRecordPos> DB::AppendLogRecord(std::unique_ptr<LogRecord> logRecord) {
    std::unique_lock<std::shared_mutex> lock(RWMutex);
    if(this->activeFile == nullptr) {
        this->SetActiveDataFile();
    }
    std::vector<byte> encodeRecord = Code::EncodeLogRecord(std::move(logRecord));
    int size = encodeRecord.size();
    if(this->activeFile->WriteOff + size > this->option.DataFileSize) {
        this->activeFile->Sync();
        this->olderFiles[this->activeFile->FileId] = std::move(this->activeFile);
        this->SetActiveDataFile();
    }

    auto writeOff = this->activeFile->WriteOff;
    this->activeFile->Write(encodeRecord);

    if(this->option.SyncWrite) {
        this->activeFile->Sync();
    }
    return std::make_unique<LogRecordPos>(this->activeFile->FileId, writeOff);
}

void DB::SetActiveDataFile() {
    uint32 initialFieldId = 0;
    if(this->activeFile != nullptr) {
        initialFieldId = this->activeFile->FileId + 1;
    }
    auto dataFile = this->activeFile->OpenDataFile(this->option.DirPath, initialFieldId);
    if(dataFile == nullptr) {
        throw "DB::SetActiveDataFile no such datafile";
    }
    this->activeFile = std::move(dataFile);

}