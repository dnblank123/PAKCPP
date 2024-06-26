#ifndef FILESYS_HPP
#define FILESYS_HPP

#include <filesystem>
#include <fstream>
#include <string>

#include "filestruct.hpp"

//https://godbolt.org/z/fdzd7hc9r
//https://godbolt.org/z/Me3EGhcTG
namespace fs{
    class FileStream {
    public:
        void File(const std::filesystem::path& FileLocation, ContentFiles& Files, Header& Head, TempAlloc& TmpAll);
    private:
        void WriteHeadFile(Header& Head);
        static constexpr void CopyInfo(ContentFiles& Files, TempAlloc& TmpAll, std::string& PathName);
        std::ifstream InFile;
        std::ofstream OutFile;
    };
}

#endif