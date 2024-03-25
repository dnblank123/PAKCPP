#pragma once

#include <filesystem>
#include <fstream>

#include "filestruct.h"

class FileStream {
public:
    void File(const std::filesystem::path& FileLocation, ContentFiles& Files, Header& Head);
    void WriteHeadFile(Header& Head);
private:
    std::ifstream InFile;
    std::ofstream OutFile;
};