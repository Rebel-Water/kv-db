#pragma once
#include "Type.hpp"
#include "IOManager.hpp"
#include <string>
#include <memory>

class DataFile {
public:             // write buf but read logRecord, maybe can do encode and decode in this
    DataFile(std::string& path, uint32 fileId) {

    }
    uint32 FileId;
    int64 WriteOff;
    IOManager IoManager;
    
    void Sync() {
        return ;
    }

    void Write(const std::vector<byte>& buf) {
        return ;
    }


};