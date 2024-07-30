#include "FileIO.hpp"
FileIO::FileIO(const std::string &fileName)
{
    fd.open(fileName, std::ios::in | std::ios::out | std::ios::app);
    if (!fd.is_open())
        throw std::system_error(errno, std::generic_category(), "Failed to open file: " + fileName);
}

// Read function
std::streamsize FileIO::Read(std::vector<byte> &buffer, int64 offset)
{
    fd.seekg(offset);
    int n = buffer.size();
    fd.read(reinterpret_cast<char *>(buffer.data()), n);
    return fd.gcount();
}

// Write function
std::streamsize FileIO::Write(const std::vector<byte> &buffer)
{
    int n = buffer.size();
    fd.write(reinterpret_cast<const char *>(buffer.data()), n);
    return n;
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

// int FileIO::GetFd()
// {
//     auto helper = [](std::filebuf &fb) -> int
//     {
//         class Helper : public std::filebuf
//         {
//             public:
//             int handle() { return _M_file.fd(); }
//         };
//         return static_cast<Helper&>(fb).handle();
//     };
//     return helper(*(this->fd.rdbuf()));
// }

// Size function
std::streamsize FileIO::Size()
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