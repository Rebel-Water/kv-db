#include "DataFile.hpp"
#include "Type.hpp"
#include <vector>

DataFile::DataFile(const std::string &path, uint32 fileId, DataFileType type, IOType iotype) 
    : FileId(fileId), WriteOff(0)
{
    std::filesystem::path fileName(path);
    switch (type)
    {
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
    if (iotype == IOType::FileIOType)
        IoManager = std::make_unique<FileIO>(fileName);
    else
        IoManager = std::make_unique<Mmap>(fileName);
}

std::string DataFile::GetDataFileName(const std::filesystem::path &dirPath, uint32 fileId)
{
    std::stringstream ss;
    ss << std::setw(9) << std::setfill('0') << std::right << fileId << DataFileNameSuffix;
    return dirPath / ss.str();
}

void DataFile::Sync()
{
    this->IoManager->Sync();
}

void DataFile::Close()
{
    this->IoManager->Close();
}

void DataFile::Write(const std::vector<byte> &buf)
{
    int n = this->IoManager->Write(buf);
    this->WriteOff += n;
}

std::vector<byte> DataFile::readNBytes(int64 n, int64 offset)
{
    std::vector<byte> buf(n);
    int size = this->IoManager->Read(buf, offset);
    return std::move(buf);
}
