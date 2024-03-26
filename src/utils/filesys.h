#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "filestruct.h"

class FileStream {
public:
    void File(const std::filesystem::path& FileLocation, ContentFiles& Files, Header& Head, TempAlloc& TmpAll);
    void WriteHeadFile(Header& Head);
    void CopyInfo(ContentFiles& Files, TempAlloc& TmpAll, std::string& PathName);
private:
    std::ifstream InFile;
    std::ofstream OutFile;
};