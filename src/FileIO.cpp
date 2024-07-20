#include "FileIO.hpp"
FileIO::FileIO(const std::string &fileName)
{
    fd.open(fileName, std::ios::in | std::ios::out | std::ios::app);
    if (!fd.is_open())
        throw std::system_error(errno, std::generic_category(), "Failed to open file: " + fileName);
}

// Read function
std::streamsize FileIO::Read(char *buffer, std::streamsize size, std::streamoff offset)
{
    fd.seekg(offset);
    fd.read(buffer, size);
    return fd.gcount();
}

// Write function
std::streamsize FileIO::Write(const char *buffer, std::streamsize size)
{
    fd.write(buffer, size);
    return size;
}

// Sync function
void FileIO::Sync()
{
    fd.flush();
}

// Close function
void FileIO::Close()
{
    if (fd.is_open())
        fd.close();
}

// Size function
std::streamoff FileIO::Size()
{
    std::streamoff currentPos = fd.tellg();
    fd.seekg(0, std::ios::end);
    std::streamoff fileSize = fd.tellg();
    fd.seekg(currentPos, std::ios::beg);
    return fileSize;
}

// Destructor
FileIO::~FileIO()
{
    Close();
}