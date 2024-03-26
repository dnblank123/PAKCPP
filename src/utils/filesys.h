#pragma once

#include <filesystem>
#include <fstream>
#include <string>

#include "filestruct.h"

//https://godbolt.org/z/fdzd7hc9r
class FileStream {
public:
    void File(const std::filesystem::path& FileLocation, ContentFiles& Files, Header& Head, TempAlloc& TmpAll);
private:
    void WriteHeadFile(Header& Head);
    static void CopyInfo(ContentFiles& Files, TempAlloc& TmpAll, std::string& PathName);
    std::ifstream InFile;
    std::ofstream OutFile;
};