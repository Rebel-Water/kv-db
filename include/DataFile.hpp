#pragma once
#include "Type.hpp"
#include "IOManager.hpp"
#include "FileIO.hpp"
#include "Mmap.hpp"
#include "Code.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <filesystem>
#include <memory>

class DataFile {
public:             // write buf but read logRecord, maybe can do encode and decode in this
    DataFile(const std::string& path, uint32 fileId = 0, DataFileType type = DataFileType::DataNormalFile, IOType iotype = IOType::FileIOType) : FileId(fileId), WriteOff(0) {
        std::filesystem::path fileName(path);
        switch(type) {
            case DataFileType::DataNormalFile:
                fileName = GetDataFileName(fileName, fileId);
                break;
            case DataFileType::HintFile:
                fileName /= HintFileName;
                break;
            case DataFileType::MergeFinishFile:
                fileName /= MergeFinishedFileName;
                break;
            default:
                throw std::runtime_error("DataFile::DataFile Undefined Type");
        }
        if(iotype == IOType::FileIOType) 
            IoManager = std::make_unique<FileIO>(fileName);
        else 
            IoManager = std::make_unique<Mmap>(fileName);
    }

    static std::string GetDataFileName(const std::filesystem::path& dirPath, uint32 fileId) {
        std::stringstream ss;
        ss << std::setw(9) << std::setfill('0') << std::right << fileId << DataFileNameSuffix;
        return dirPath / ss.str();
    }
    
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
        // buf.resize(size);
        return buf;
    }

    private:
    uint32 FileId;
    int64 WriteOff;
    std::unique_ptr<IOManager> IoManager;
    friend class DB;
};