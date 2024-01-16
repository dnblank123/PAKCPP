#include <iostream>
#include <filesystem>
#include <fstream>
#include "zopfli.h"
#include <algorithm>
#include <fstream>
#pragma comment(lib, "zopfli.lib")

// Generate header of file
// Get files on folder then count total
// Usage pakcpp output.pak folder1 folder2
// inserting : use tellp to check positioning
typedef struct
{
    int FileCount;
} pakheader;
int GetFilesFolder(std::filesystem::path &FileLoc)
{
    size_t filecount = 0;
    std::filesystem::path file{ FileLoc };
    std::cout << "List of files: \n";
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc })
    {
        if (dir_entry.path().has_extension())
        {
            filecount++;
            std::cout << dir_entry.path() << '\n';
        }
    }
    std::cout << "Total files: " << filecount << '\n';
    return 0;
}

void FileCompress()
{
    unsigned char* in = 0;
    size_t insize = 0;
    unsigned char* out = 0;
    size_t outsize = 0;
    ZopfliCompress(NULL, ZOPFLI_FORMAT_ZLIB , in, insize, &out, &outsize);
}

int main(int argc, char** argv)
{
    std::filesystem::path argvpath;

    if (argc == 1 || argc > 4)
    {
        std::cout << "Usage: pakcpp.exe filename folder1 folder2" << '\n';
        exit(0);
    }
    std::string pakfile = argv[1];

    if (!pakfile.contains(".pak")) //better to check last four characters
    {
        pakfile += ".pak";
    }

    for (int i = 2; i < argc; i++) 
    {
        argvpath = argv[i];
    }

    GetFilesFolder(argvpath);
    {
        std::ofstream ostrm(pakfile, std::ios::binary);
        double d = 3.14;
        ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // binary output
    }

    return 0;
}
