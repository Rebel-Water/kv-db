#include <iostream>
#include <filesystem>
#include <sys/statvfs.h>
#include <unistd.h>
#include "Type.hpp"
#include <string>
#include <gtest/gtest.h>

class File
{
public:
    // Function to get the size of a directory
    static int64 DirSize(const std::filesystem::path& dirPath)
    {
        int64 size = 0;
        for (const auto &entry : std::filesystem::recursive_directory_iterator(dirPath)) 
                if(std::filesystem::is_regular_file(entry))
                    size += std::filesystem::file_size(entry);
        return size;
    }

    // Function to get the available disk space
    static uint64 AvailableDiskSize()
    {
        struct statvfs stat;
        if (statvfs(defaultPath.c_str(), &stat) != 0)
            throw std::runtime_error("AvailableDiskSize");
        return stat.f_bavail * stat.f_bsize;
    }

    static void CopyDir(const std::string &src, const std::string &dest, const std::vector<std::string> &exclude)
    {
        auto match_pattern = [&](const std::string &pattern, const std::string &str)
        {
            return str.find(pattern) != std::string::npos;
        };

        if (!std::filesystem::exists(dest))
            std::filesystem::create_directories(dest);

        for (const auto &entry : std::filesystem::recursive_directory_iterator(src))
        {
            const auto &path = entry.path();
            auto relative_path = std::filesystem::relative(path, src);
            auto dest_path = dest / relative_path;

            std::string file_name = path.filename().string();

            for (const auto &pattern : exclude)
            {
                if (match_pattern(pattern, file_name))
                    goto end_loop;
            }

            if (entry.is_directory())
                std::filesystem::create_directories(dest_path);
            else if (entry.is_regular_file())
                std::filesystem::copy_file(path, dest_path, std::filesystem::copy_options::overwrite_existing);
            end_loop:
            ;
        }
    }
};