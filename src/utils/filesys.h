#pragma once

#include <filesystem>
#include <fstream>

#include "filestruct.h"

class FileStream {
public:
    void File(const std::filesystem::path &FileLocation, ContentFiles &Files);
    void CloseFile();
private:
    std::ifstream InFile;
    std::ofstream OutFile;
};