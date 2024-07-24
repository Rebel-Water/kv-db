#pragma once
#include "Type.hpp"
#include "IOManager.hpp"
#include "FileIO.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <memory>

class DataFile {
public:             // write buf but read logRecord, maybe can do encode and decode in this
    DataFile(const std::string& path, uint32 fileId) : FileId(fileId), WriteOff(0) {
        std::stringstream ss;
        ss << path << std::setw(9) << std::setfill('0') << std::right << fileId << DataFileNameSuffix;
        std::string fileName = ss.str();
        IoManager = std::make_unique<FileIO>(fileName);
    }
    uint32 FileId;
    int64 WriteOff;
    std::unique_ptr<IOManager> IoManager;
    
    void Sync() {
        this->IoManager->Sync();
    }

    void Close() {
        this->IoManager->Close();
    }

    void Write(const std::vector<byte>& buf) {
        int n = this->IoManager->Write(buf);
        this->WriteOff += n;
    }

    std::vector<byte> readNBytes(int64 n, int64 offset) {
        std::vector<byte> buf(n);
        int size = this->IoManager->Read(buf, offset);
        return buf;
    }


};