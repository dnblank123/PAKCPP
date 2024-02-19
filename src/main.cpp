#include <iostream>
#include <filesystem>
#include <fstream>
#include "zopfli.h"
#include <algorithm>
#include <fstream>
#include <processthreadsapi.h>
#include <Windows.h>
#include <WinBase.h>
#pragma comment(lib, "zopfli.lib")

// Generate header of file
// Get files on folder then count total
// Usage pakcpp output.pak folder1 folder2
// inserting : use tellp to check positioning
typedef struct
{
    int FileCount;
} pakheader;

std::string rtrim_null(std::string& str) {
    str.erase(std::find_if(str.rbegin(), str.rend(), [](int character) {
        return '\0' != character;
        }).base(), str.end());
}

int GetFilesFolder(std::filesystem::path &FileLoc)
{
    size_t filecount = 0;
    std::filesystem::path file{ FileLoc };
    std::cout << "List of files: \n";
    ZopfliOptions options;

    ZopfliInitOptions(&options);
    options.numiterations = 30;
    options.blocksplittingmax = 0;
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc })
    {
        if (dir_entry.path().has_extension())
        {
            filecount++;
            std::cout << dir_entry.path() << '\n';
            std::filesystem::path file2 = dir_entry.path();
            //probably compress here?
            std::ifstream stream;//(file2.string().c_str(), std::ios::binary);
            std::ofstream newfile("my_file_name", std::ios::binary | std::ios::out | std::ios::app);
            stream.open(file2, std::ios::binary); //blud std::ios::in wrong input
            std::cout << stream.tellg() << '\n';
            stream.seekg(0, std::ios::end);
            size_t filesize = stream.tellg();
            stream.seekg(0, std::ios::beg);
            std::vector<unsigned char> buffer(filesize);
            size_t compressedsize = 0;
            unsigned char* compressedbuffer = 0;
            std::vector<const char*> pathemptybytes(256 - file2.string().length());
            std::vector<const char*> emptybytes(44);
            if (newfile) 
            {
                //TODO: fix naming
                stream.read(std::bit_cast<char*>(buffer.data()), filesize);
                ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, buffer.data(), filesize, &compressedbuffer, &compressedsize);
                newfile.write(reinterpret_cast<const char*>(emptybytes.data()), emptybytes.size()); //blank 44 bytes
                size_t pos = newfile.tellp(); // pos
                newfile.write(std::bit_cast<const char*>(compressedbuffer), compressedsize); //compressed file
                newfile.write(file2.string().c_str(), file2.string().size()); // path
                newfile.write(std::bit_cast<const char*>(pathemptybytes.data()), pathemptybytes.size()); // path empty bytes
                newfile.write(reinterpret_cast<const char*>(&compressedsize), sizeof(static_cast<int>(compressedsize))); //raw size
                newfile.write(reinterpret_cast<const char*>(&filesize), sizeof(static_cast<int>(filesize))); //uncompressed filesize
                newfile.write(reinterpret_cast<const char*>(&compressedsize), sizeof(static_cast<int>(compressedsize))); //compressed size
                newfile.write(reinterpret_cast<const char*>(&pos), sizeof(static_cast<int>(pos))); //file offset
                //file offset
                newfile.write(reinterpret_cast<const char*>(emptybytes.data()), emptybytes.size()); //blank 44 bytes

                //write path for 256 bytes // 256 - file.string().c_str()
                //write raw size 4 bytes //compressedsize
                //write real size of the file before compression 4 bytes //size_t filesize = stream.tellg();
                //write compressed size 4 bytes //compressedsize
                //write file offset 4 bytes // idk
                //write blank 4 bytes
                //write blank 40 bytes
                std::cout << "File compressed and written successfully\n";
            }
            free(compressedbuffer);
            std::cout << file2.string().c_str();
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
//void FileCompress()
//{
//    unsigned char* in = 0;
//    size_t insize = 0;
//    unsigned char* out = 0;
//    size_t outsize = 0;
//    ZopfliCompress(NULL, ZOPFLI_FORMAT_ZLIB , in, insize, &out, &outsize);
//}

int main(int argc, char** argv)
{
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();
    SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
    SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
    std::filesystem::path argvpath;

    if (argc == 2 || argc > 4)
    {
        std::cout << "Usage: pakcpp.exe filename folder1 folder2" << '\n';
        exit(0);
    }
    std::string pakfile = argv[1];

    //if (!pakfile.contains(".pak")) //better to check last four characters
    //{
    //    pakfile += ".pak";
    //}

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
