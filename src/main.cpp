#include <Windows.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <fstream>
#include <iostream>
#include <processthreadsapi.h>
#include <vector>

#include "src/zopfli/zopfli.h"

#ifdef _MSC_VER
#pragma comment(lib, "zopfli.lib")
#endif
#include <variant>
#include <any>

int GetFilesFolderAndCompress(std::filesystem::path& FileLoc)
{
    size_t filecount = 0;
    //std::cout << "List of files: \n";
    ZopfliOptions options;
    ZopfliInitOptions(&options);
    options.numiterations = 100;
    bool once = false;

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLoc }) {
        if (dir_entry.path().has_extension()) {
            filecount++;
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
            std::ifstream stream;
            std::ofstream newfile("00ResourceTemp1.pak", std::ios::binary | std::ios::out | std::ios::app);
            std::ifstream posfile("00ResourceTemp1.pak", std::ios::binary);
            const char startbytes[1024]{};
            stream.open(file2, std::ios::binary);
            stream.seekg(0, std::ios::end);
            size_t filesize = stream.tellg();
            stream.seekg(0, std::ios::beg);
            std::vector<unsigned char> buffer(filesize);
            size_t compressedsize = 0;
            unsigned char* compressedbuffer = 0;
            size_t pos = 0x400;
            std::vector<std::byte> tempbuffer;
            std::vector<const char*> pathemptybytes(256 - modifiedpath.length());
            std::vector<int> te = { 1, 2, 3 };
            const char emptybytes[44]{};
            if (newfile) {
                //TODO: fix naming
                stream.read(std::bit_cast<char*>(buffer.data()), filesize);
                //std::ofstream newfile2("00ResourceTemp2.pak", std::ios::binary | std::ios::out | std::ios::app);
                if (!once) {
                    once = true;
                    newfile.write(startbytes, sizeof(startbytes));
                }
                ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, buffer.data(), filesize, &compressedbuffer, &compressedsize);
                newfile.write(std::bit_cast<const char*>(compressedbuffer), compressedsize); //compressed file

                if (filecount != 1) {
                    posfile.seekg(0, std::ios::end); // pos
                    pos = posfile.tellg();
                    //std::cout << pos << '\n';
                }
                //tempbuffer.resize(316);
                std::cout << "size: " << tempbuffer.size() << " capacity: " << tempbuffer.capacity() << '\n';
                tempbuffer.insert(tempbuffer.end(), reinterpret_cast<const std::byte*>(modifiedpath.c_str()), reinterpret_cast<const std::byte*>(modifiedpath.c_str() + modifiedpath.size()));
                tempbuffer.insert(tempbuffer.end(), reinterpret_cast<const std::byte*>(pathemptybytes.data()), reinterpret_cast<const std::byte*>(pathemptybytes.data() + pathemptybytes.size()));
                tempbuffer.resize(256);
                tempbuffer.shrink_to_fit();

                //tempbuffer.insert(tempbuffer.end(), modifiedpath.begin(), modifiedpath.end());
                //tempbuffer.insert(tempbuffer.end(), pathemptybytes.begin(), pathemptybytes.end());
                //tempbuffer.push_back(modifiedpath);
                //tempbuffer.insert(tempbuffer.end(), "test");
                //tempbuffer.push_back(pathemptybytes.data());
                //tempbuffer.push_back(compressedsize);
                //tempbuffer.push_back(filesize);
                //tempbuffer.push_back(compressedsize);
                //tempbuffer.push_back(pos);
                //tempbuffer.push_back(emptybytes);
                newfile.write(std::bit_cast<const char*>(tempbuffer.data()), tempbuffer.size());
                std::cout << "size: " << tempbuffer.size() << " capacity: " << tempbuffer.capacity() << '\n';

                //newfile2.write(modifiedpath.c_str(), modifiedpath.size()); // path
                //newfile2.write(std::bit_cast<const char*>(pathemptybytes.data()), pathemptybytes.size()); // path empty bytes
                //newfile2.write(std::bit_cast<const char*>(&compressedsize), sizeof(static_cast<unsigned int>(compressedsize))); //raw size
                //newfile2.write(std::bit_cast<const char*>(&filesize), sizeof(static_cast<unsigned int>(filesize))); //uncompressed filesize
                //newfile2.write(std::bit_cast<const char*>(&compressedsize), sizeof(static_cast<unsigned int>(compressedsize))); //compressed size
                //newfile2.write(std::bit_cast<const char*>(&pos), sizeof(static_cast<unsigned int>(pos))); //file offset
                //newfile2.write(emptybytes, sizeof(emptybytes)); //blank 44 bytes

                //std::cout << "File compressed and written successfully" << '\n';
            }
            free(compressedbuffer);
            std::cout << file2.string().c_str() << '\n';
        }

    }

    size_t endpos = 0;
    std::ofstream of_a("00ResourceTemp1.pak", std::ios_base::binary | std::ios_base::app);
    //std::ifstream if_b("00ResourceTemp2.pak", std::ios_base::binary);
    of_a.seekp(0, std::ios_base::end);
    endpos = of_a.tellp();
    //of_a << if_b.rdbuf();

    of_a.close();
    //if_b.close();

    of_a.open("00ResourceTemp1.pak", std::ios_base::binary | std::ios_base::in);
    of_a.seekp(0, std::ios_base::beg);
    of_a << "EyedentityGames Packing File 0.1";
    of_a.seekp(256);
    int8_t value = 11;
    of_a.write(std::bit_cast<char*>(&value), sizeof(value));
    of_a.seekp(260);
    of_a.write(std::bit_cast<char*>(&filecount), sizeof(static_cast<unsigned int>(filecount)));
    of_a.seekp(264);
    of_a.write(std::bit_cast<char*>(&endpos), sizeof(static_cast<unsigned int>(endpos)));
    of_a.close();

    //std::cout << "Total files: " << filecount << '\n';
    return 0;
}

int main(int argc, char** argv)
{
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();
    SetPriorityClass(hProcess, REALTIME_PRIORITY_CLASS);
    SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);

    std::filesystem::path argvpath;

    if (argc == 1 || argc > 2)
    {
        std::cout << "Usage: pakcpp.exe folder" << '\n';
        exit(0);
    }

    if (std::filesystem::exists("00ResourceTemp1.pak") || std::filesystem::exists("00Resource.pak")) {
        std::filesystem::remove("00ResourceTemp1.pak");
        std::filesystem::remove("00ResourceTemp2.pak");
        std::filesystem::remove("00Resource.pak");
        //std::cout << "file deleted" << '\n';
    }

    argvpath = argv[1];
    GetFilesFolderAndCompress(argvpath);

    if (std::filesystem::exists("00ResourceTemp2.pak")) {
        std::filesystem::remove("00ResourceTemp2.pak");
        std::filesystem::rename("00ResourceTemp1.pak", "00Resource.pak");
        //std::cout << "file deleted" << '\n';
    }
    return 0;
}
