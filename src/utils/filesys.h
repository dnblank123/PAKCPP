#pragma once

#include <fstream>
#include <string>

namespace FileSys{

	void GetFiles(const std::filesystem::path &FileLocation, ContentFiles &Files);
}

class FileStream {
public:
    void OpenFile(const std::string &FilePath);
    void CloseFile();
private:
    std::ifstream InFile;
    std::ofstream OutFiles;
};