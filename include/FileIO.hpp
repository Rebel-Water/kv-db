#include <fstream>
#include <string>
#include <system_error>
#include "IOManager.hpp"

class FileIO : public IOManager
{
private:
    std::fstream fd;
public:
    // Constructor to initialize FileIO
    FileIO(const std::string &fileName);

    // Read function
    virtual std::streamsize Read(std::vector<byte>& buf, int64 offset); // Write function
    virtual std::streamsize Write(const std::vector<byte>& buf);                 // Sync function
    virtual void Sync();                                                                     // Close function
    virtual void Close();

    // int GetFd();

    // Size function
    virtual std::streamsize Size(); // Destructor
    ~FileIO();
};
