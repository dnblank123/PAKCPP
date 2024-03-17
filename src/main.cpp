#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "zopfli.h"

void GetFilesFolderAndCompress(std::filesystem::path& FileLoc);

struct Header {
    std::string filesignature = "EyedentityGames Packing File 0.1";
    size_t filecount = 0;
    static constexpr uint8_t unknown = 11;
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
};

struct TempAlloc {
    std::vector<char> tempbuffer = {};
};

void GetFilesFolderAndCompress(std::filesystem::path& FileLoc)
{
    Header header;
    ContentFiles contentfiles;
    TempAlloc tempalloc;  

    ZopfliOptions options;
    ZopfliInitOptions(&options);
    options.numiterations = 100;
    const std::string& rootpath = FileLoc.string();
    std::ifstream stream;
    std::ofstream file("00Resource.pak", std::ios::binary | std::ios::out | std::ios::app);

    file.write(header.headerbytes.data(), header.headerbytes.size());

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc }) {
        if (dir_entry.path().has_extension()) {
            header.filecount++;
        }
    }

    tempalloc.tempbuffer.reserve(316 * header.filecount);
    contentfiles.pathemptybytes.reserve(256 * header.filecount);

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc }) {
        if (dir_entry.path().has_extension()) {

            const std::filesystem::path& filepath = dir_entry.path();
            std::string path = filepath.string();
            if (path.find(rootpath) == 0) {
                path.erase(0, rootpath.length());
            }
            //std::cout << path << '\n';


            stream.open(filepath, std::ios::binary);
            stream.seekg(0, std::ios::end);
            contentfiles.filesize = static_cast<size_t>(stream.tellg());
            stream.seekg(0, std::ios::beg);

            contentfiles.incompress.reserve(contentfiles.filesize);

            contentfiles.pathemptybytes.resize(256 - path.length());

            //TODO: fix naming
            stream.read(std::bit_cast<char*>(contentfiles.incompress.data()), static_cast<uint32_t>(contentfiles.filesize));

            contentfiles.filepos = static_cast<size_t>(file.tellp());
                
            ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, contentfiles.incompress.data(), contentfiles.filesize, &contentfiles.compressedbuffer, &contentfiles.compressedsize);
            file.write(std::bit_cast<const char*>(contentfiles.compressedbuffer), static_cast<uint32_t>(contentfiles.compressedsize));

            //std::cout << "size: " << tempalloc.tempbuffer.size() << " capacity: " << tempalloc.tempbuffer.capacity() << '\n';
            //auto timer1 = std::chrono::high_resolution_clock::now();

            std::copy(path.begin(), 
                        path.end(), 
                            std::back_inserter(tempalloc.tempbuffer));
                
            std::copy(contentfiles.pathemptybytes.begin(), 
                        contentfiles.pathemptybytes.end(), 
                            std::back_inserter(tempalloc.tempbuffer));
               
            std::copy(std::bit_cast<const char*>(&contentfiles.compressedsize),
                        std::bit_cast<const char*>(&contentfiles.compressedsize) + sizeof(uint32_t),
                            std::back_inserter(tempalloc.tempbuffer));

            std::copy(std::bit_cast<const char*>(&contentfiles.filesize),
                        std::bit_cast<const char*>(&contentfiles.filesize) + sizeof(uint32_t),
                            std::back_inserter(tempalloc.tempbuffer));

            std::copy(std::bit_cast<const char*>(&contentfiles.compressedsize),
                        std::bit_cast<const char*>(&contentfiles.compressedsize) + sizeof(uint32_t),
                            std::back_inserter(tempalloc.tempbuffer));

            std::copy(std::bit_cast<const char*>(&contentfiles.filepos),
                        std::bit_cast<const char*>(&contentfiles.filepos) + sizeof(uint32_t),
                            std::back_inserter(tempalloc.tempbuffer));

            std::copy(contentfiles.emptybytes.begin(),
                        contentfiles.emptybytes.end(),
                            std::back_inserter(tempalloc.tempbuffer));


            //auto timer2 = std::chrono::high_resolution_clock::now();

            //const std::chrono::duration<double, std::milli> ms_double = timer2 - timer1;

            //std::cout << "Time :  " << ms_double.count() << "ms" << '\n';
            
        }
        contentfiles.incompress.clear();
        contentfiles.compressedsize = 0;
        contentfiles.compressedbuffer = nullptr;
        contentfiles.filesize = 0;
        contentfiles.filepos = 0;
        stream.close();
    }
    header.fileindex = static_cast<uint32_t>(file.tellp());
    file.write(std::bit_cast<const char*>(tempalloc.tempbuffer.data()), std::bit_cast<std::streamsize>(tempalloc.tempbuffer.size()));

    file.close();

    file.open("00Resource.pak", std::ios_base::binary | std::ios_base::in);
    file.write(header.filesignature.c_str(), header.filesignature.size());
    file.seekp(256);
    file.write(std::bit_cast<char*>(&header.unknown), sizeof(header.unknown));
    file.seekp(260);
    file.write(std::bit_cast<char*>(&header.filecount), sizeof(static_cast<uint32_t>(header.filecount)));
    file.seekp(264);
    file.write(std::bit_cast<char*>(&header.fileindex), sizeof(static_cast<uint32_t>(header.fileindex)));
    file.close();

    //std::cout << "size: " << tempalloc.tempbuffer.size() << " capacity: " << tempalloc.tempbuffer.capacity() << '\n';
}

int main(int argc, char** argv)
{
    std::filesystem::path argvpath;

    if (argc == 1 || argc > 2)
    {
        std::cout << "Usage: pakcpp.exe folder" << '\n';
        std::quick_exit(0);
    }

    if (std::filesystem::exists("00Resource.pak")) {
        std::filesystem::remove("00Resource.pak");
    }

    argvpath = argv[1];
    GetFilesFolderAndCompress(argvpath);


    return 0;
}
