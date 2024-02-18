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
    ZopfliOptions options;
    ZopfliInitOptions(&options);
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc })
    {
        if (dir_entry.path().has_extension())
        {
            filecount++;
            std::cout << dir_entry.path() << '\n';
            std::filesystem::path file2 = dir_entry.path();
            //probably compress here?
            std::ifstream stream;//(file2.string().c_str(), std::ios::binary);
            std::fstream newfile("my_file_name", std::fstream::binary| std::fstream::out);
            stream.open(file2, std::ios::in);
            stream.seekg(0, std::ios::end);
            size_t filesize = stream.tellg();
            stream.seekg(0, std::ios::beg);
            std::vector<unsigned char*> buffer(filesize);
            size_t compressedsize = 0;
            unsigned char* compressedBuffer = nullptr;

            if (newfile) 
            {
                    stream.read(reinterpret_cast<char*>(buffer.data()), filesize);
                    ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, reinterpret_cast<const unsigned char*>(buffer.data()), filesize, &compressedBuffer, &compressedsize);
                    newfile.write(reinterpret_cast<char*>(compressedBuffer), compressedsize);
                    std::cout << "File compressed and written successfully\n";
            } 
            //std::cout << file2.string().c_str();
        }
    }
    std::cout << "Total files: " << filecount << '\n';
    return 0;
}
//int GetFilesFolder(std::filesystem::path& FileLoc)
//{
//    size_t filecount = 0;
//
//    std::filesystem::path file{ FileLoc };
//    std::cout << "List of files: \n";
//    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc }) {
//        if (dir_entry.path().has_extension()) {
//            filecount++;
//            std::cout << dir_entry.path() << '\n';
//            std::filesystem::path file2 = dir_entry.path();
//            //probably compress here?
//            //std::ifstream stream(file.string().c_str(), std::ios::binary);
//
//            std::cout << file2.string().c_str();
//            //ZopfliCompress(nullptr, ZOPFLI_FORMAT_ZLIB, in, insize, &out, &outsize);
//        }
//    }
//    std::cout << "Total files: " << filecount << '\n';
//    return 0;
//}
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

    if (argc == 2 || argc > 4)
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
    //{
    //    std::ofstream ostrm(pakfile, std::ios::binary);
    //    double d = 3.14;
    //    ostrm.write(reinterpret_cast<char*>(&d), sizeof d); // binary output
    //}

    return 0;
}
