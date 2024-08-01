#include "Mmap.hpp"
Mmap::Mmap(const std::string& fileName) : fd(-1), data(nullptr), length(0) {
    fd = open(fileName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        throw std::runtime_error("Mmap::Mmap Failed to open file");
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        throw std::runtime_error("Mmap::Mmap Failed to get file size");
    }
    length = sb.st_size;

    data = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Mmap::Mmap Failed to mmap file");
    }
}

Mmap::~Mmap() {
    Close();
}

std::streamsize Mmap::Read(std::vector<byte> &buf, int64 offset)
{
    int size = buf.size();
    if(offset + size > length)
        size = length - offset;
    std::memcpy(reinterpret_cast<char*>(buf.data()), static_cast<char*>(data) + offset, size);
    return size;
}

void Mmap::Close()
{
    if (data != nullptr && data != MAP_FAILED) {
        munmap(data, length);
        data = nullptr;
    }
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}
