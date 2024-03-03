#include <algorithm>
#include <any>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <processthreadsapi.h>
#include <vector>
#include <Windows.h>
#include <array>
#include "src/zopfli/zopfli.h"
#include <string_view>
#ifdef _MSC_VER
#pragma comment(lib, "zopfli.lib")
#endif
void GetFilesFolderAndCompress(std::filesystem::path& FileLoc);

struct Header {
    std::string filesignature = "EyedentityGames Packing File 0.1";
    size_t filecount = 0;
    static constexpr uint8_t value = 11;
    std::array<const char, 1024> headerbytes = {};
    size_t fileindex = 0;
};

struct ContentFiles {
    std::vector<unsigned char> incompress = {};
    size_t compressedsize = 0;
    unsigned char* compressedbuffer = nullptr;
    std::vector<char> pathemptybytes = {};
    size_t filepos = 0;
    size_t filesize = 0;
    std::array<uint8_t, 44> emptybytes = {};

    ~ContentFiles(){
        incompress.clear();
        compressedsize = 0;
        compressedbuffer = nullptr;
        filesize = 0;
        filepos = 0;
    }
};

struct TempAlloc {
    std::vector<std::byte> tempbuffer = {};
    std::uint8_t* compressedsizeptr = nullptr;
    std::uint8_t* filesizeptr = nullptr;
    std::uint8_t* posptr = nullptr;
};

//possible speed up : count total files then reserve once.
void GetFilesFolderAndCompress(std::filesystem::path& FileLoc)
{
    Header header;
    ContentFiles contentfiles;
    TempAlloc tempalloc;  

    ZopfliOptions options;
    ZopfliInitOptions(&options);
    options.numiterations = 100;

    std::ifstream stream;
    std::ofstream newfile("00Resource.pak", std::ios::binary | std::ios::out | std::ios::app);

    newfile.write(header.headerbytes.data(), header.headerbytes.size());
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc }) {
        if (dir_entry.path().has_extension()) {
            header.filecount++;
        }
    }

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc }) {
        if (dir_entry.path().has_extension()) {
            std::cout << dir_entry.path() << '\n';
            std::filesystem::path file2 = dir_entry.path();
            std::string modifiedpath = "\\";
            bool isFirstComponent = true;
            for (const auto& component : file2) {
                if (isFirstComponent) {
                    isFirstComponent = false;
                    continue;
                }
                modifiedpath += component.string() + "\\";
            }
            modifiedpath.pop_back();

            stream.open(file2, std::ios::binary);
            stream.seekg(0, std::ios::end);
            contentfiles.filesize = static_cast<size_t>(stream.tellg());
            stream.seekg(0, std::ios::beg);

            contentfiles.incompress.resize(contentfiles.filesize);

            contentfiles.pathemptybytes.resize(256 - modifiedpath.length());
            contentfiles.pathemptybytes.reserve(256 * header.filecount);
            
            tempalloc.tempbuffer.reserve(316 * header.filecount);

            if (newfile) {
                //TODO: fix naming
                stream.read(std::bit_cast<char*>(contentfiles.incompress.data()), static_cast<uint32_t>(contentfiles.filesize));

                contentfiles.filepos = static_cast<size_t>(newfile.tellp());
                tempalloc.posptr = std::bit_cast<std::uint8_t*>(&contentfiles.filepos);
                
                ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, contentfiles.incompress.data(), contentfiles.filesize, &contentfiles.compressedbuffer, &contentfiles.compressedsize);
                newfile.write(std::bit_cast<const char*>(contentfiles.compressedbuffer), static_cast<uint32_t>(contentfiles.compressedsize));

                tempalloc.compressedsizeptr = std::bit_cast<std::uint8_t*>(&contentfiles.compressedsize);
                tempalloc.filesizeptr = std::bit_cast<std::uint8_t*>(&contentfiles.filesize);

                std::cout << "size: " << tempalloc.tempbuffer.size() << " capacity: " << tempalloc.tempbuffer.capacity() << '\n';
                auto t1 = std::chrono::high_resolution_clock::now();
                //TODO: memcpy if I understand it...
                for (char &path : modifiedpath) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(path));
                }
                for (char &empbytes : contentfiles.pathemptybytes) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(empbytes));
                }
                for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(tempalloc.compressedsizeptr[i]));
                }
                for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(tempalloc.filesizeptr[i]));
                }
                for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(tempalloc.compressedsizeptr[i]));
                }                
                for (uint32_t i = 0; i < sizeof(uint32_t); i++) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(tempalloc.posptr[i]));
                }
                for (const char& c : contentfiles.emptybytes) {
                    tempalloc.tempbuffer.emplace_back(static_cast<std::byte>(c));
                }
                auto t2 = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::milli> ms_double = t2 - t1;

                std::cout << "Time :  " << ms_double.count() << "ms" << '\n';
            }

            std::cout << file2.string().c_str() << '\n';
        }
        contentfiles.~ContentFiles();
        stream.close();
    }
    newfile.close();
    size_t endpos = 0;
    std::ofstream of_a("00Resource.pak", std::ios_base::binary | std::ios_base::app);
    of_a.seekp(0, std::ios_base::end);
    endpos = of_a.tellp();
    of_a.write(std::bit_cast<const char*>(tempalloc.tempbuffer.data()), tempalloc.tempbuffer.size());

    of_a.close();

    newfile.open("00Resource.pak", std::ios_base::binary | std::ios_base::in);
    newfile.seekp(std::ios_base::beg);
    newfile << "EyedentityGames Packing File 0.1";
    newfile.seekp(256);
    static constexpr uint8_t value = 11;
    newfile.write(std::bit_cast<char*>(&value), sizeof(value));
    newfile.seekp(260);
    newfile.write(std::bit_cast<char*>(&header.filecount), sizeof(static_cast<unsigned int>(header.filecount)));
    newfile.seekp(264);
    newfile.write(std::bit_cast<char*>(&endpos), sizeof(static_cast<unsigned int>(endpos)));
    newfile.close();
}

int main(int argc, char** argv)
{
    HANDLE hThread = GetCurrentThread();
    SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
    std::filesystem::path argvpath;

    if (argc == 1 || argc > 2)
    {
        std::cout << "Usage: pakcpp.exe folder" << '\n';
        exit(0);
    }

    if (std::filesystem::exists("00Resource.pak")) {
        std::filesystem::remove("00Resource.pak");
        //std::cout << "file deleted" << '\n';
    }

    argvpath = argv[1];
    GetFilesFolderAndCompress(argvpath);


    return 0;
}
