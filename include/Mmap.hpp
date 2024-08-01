#pragma once
#include <iostream>
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdexcept>
#include <cstring>
#include "IOManager.hpp"

class Mmap : public IOManager {
public:
    Mmap(const std::string& fileName);
    ~Mmap();

    virtual std::streamsize Read(std::vector<byte>& buf, int64 offset);
    virtual std::streamsize Write(const std::vector<byte>& buf) {
         throw std::runtime_error("Mmap::Write Not Implemented");
    } // Not implemented
    virtual void Sync() {
         throw std::runtime_error("Mmap::Sync Not Implemented");
    } // Not implemented
    virtual void Close();
    virtual std::streamsize Size() {
        return length;
    }

private:
    int fd;
    void* data;
    size_t length;
};

