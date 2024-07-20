#include <fstream>
#include <string>
#include <system_error>

class FileIO
{
private:
    std::fstream fd;

public:
    // Constructor to initialize FileIO
    FileIO(const std::string &fileName);

    // Read function
    std::streamsize Read(char *buffer, std::streamsize size, std::streamoff offset); // Write function
    std::streamsize Write(const char *buffer, std::streamsize size);                 // Sync function
    void Sync();                                                                     // Close function
    void Close();

    // Size function
    std::streamoff Size(); // Destructor
    ~FileIO();
};
