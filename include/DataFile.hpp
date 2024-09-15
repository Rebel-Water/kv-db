#pragma once
#include <vector>
#include <iomanip>
#include <sstream>
#include <string>
#include <filesystem>
#include <memory>
#include "Type.hpp"
#include "IOManager.hpp"
#include "FileIO.hpp"
#include "Mmap.hpp"
#include "Code.hpp"

class DataFile {
public:             // write buf but read logRecord, maybe can do encode and decode in this
    DataFile(const std::string& path, uint32 fileId = 0, DataFileType type = DataFileType::DataNormalFile, IOType iotype = IOType::FileIOType);

    static std::string GetDataFileName(const std::filesystem::path& dirPath, uint32 fileId);
    void Sync();
    void Close();
    void Write(const std::vector<byte>& buf);
    std::vector<byte> readNBytes(int64 n, int64 offset);

    private:
    uint32 FileId;
    int64 WriteOff;
    std::unique_ptr<IOManager> IoManager;
    friend class DB;
};