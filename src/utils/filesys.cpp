#include <algorithm>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>

#include "compress.h"
#include "filestruct.h"
#include "filesys.h"

inline constexpr auto PATH_BYTES_RESIZE = 256;

void FileStream::File(const std::filesystem::path& FileLocation, ContentFiles& Files, Header& Head, TempAlloc& TmpAll) {
    const std::string rootpath = FileLocation.string();

    OutFile.open("00Resource.pak", std::ios::binary | std::ios::out | std::ios::app);

    OutFile.write(Head.headerbytes.data(), Head.headerbytes.size());

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLocation }) {
        if (dir_entry.path().has_extension()) {
            Head.filecount++;
        }
    }

    TmpAll.tempbuffer.reserve(316 * Head.filecount);
    Files.pathemptybytes.reserve(256 * Head.filecount);

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLocation }) {
        if (dir_entry.path().has_extension()) {
            
            const std::filesystem::path &filepath = dir_entry.path();
            std::string path = filepath.string();
            if (path.find(rootpath) == 0) {
                path.erase(0, rootpath.length());
            }

            InFile.open(filepath, std::ios::binary);

            InFile.seekg(0, std::ios::end);
            Files.filesize = static_cast<std::uint32_t>(InFile.tellg());
            InFile.seekg(0, std::ios::beg);

            Files.pathemptybytes.resize(PATH_BYTES_RESIZE - path.length());
            Files.incompress.reserve(Files.filesize);

            InFile.read(std::bit_cast<char*>(Files.incompress.data()), static_cast<std::uint32_t>(Files.filesize));

            Files.filepos = static_cast<std::uint32_t>(OutFile.tellp());

            Compress::CompFile(Files);
            OutFile.write(std::bit_cast<const char*>(Files.compressedbuffer), static_cast<std::uint32_t>(Files.compressedsize));
            CopyInfo(Files, TmpAll, path);
        }
        Compress::CompReset(Files);
        InFile.close();
    }
    Head.fileindex = static_cast<std::uint32_t>(OutFile.tellp());
    OutFile.write(std::bit_cast<const char*>(TmpAll.tempbuffer.data()), std::bit_cast<std::streamsize>(TmpAll.tempbuffer.size()));
    OutFile.close();
}

void FileStream::WriteHeadFile(Header& Head) {
    OutFile.open("00Resource.pak", std::ios_base::binary | std::ios_base::in);
    OutFile.write(Head.filesignature, strlen(Head.filesignature));
    OutFile.seekp(256);
    OutFile.write(std::bit_cast<char*>(&Head.unknown), sizeof(Head.unknown));
    OutFile.seekp(260);
    OutFile.write(std::bit_cast<char*>(&Head.filecount), sizeof(static_cast<uint32_t>(Head.filecount)));
    OutFile.seekp(264);
    OutFile.write(std::bit_cast<char*>(&Head.fileindex), sizeof(static_cast<uint32_t>(Head.fileindex)));
    OutFile.close();
}

void FileStream::CopyInfo(ContentFiles& Files, TempAlloc& TmpAll, std::string& PathName) {
    std::copy(PathName.begin(),
        PathName.end(),
        std::back_inserter(TmpAll.tempbuffer));

    std::copy(Files.pathemptybytes.begin(),
        Files.pathemptybytes.end(),
        std::back_inserter(TmpAll.tempbuffer));

    std::copy(std::bit_cast<const char*>(&Files.compressedsize),
        std::bit_cast<const char*>(&Files.compressedsize) + sizeof(uint32_t),
        std::back_inserter(TmpAll.tempbuffer));

    std::copy(std::bit_cast<const char*>(&Files.filesize),
        std::bit_cast<const char*>(&Files.filesize) + sizeof(uint32_t),
        std::back_inserter(TmpAll.tempbuffer));

    std::copy(std::bit_cast<const char*>(&Files.compressedsize),
        std::bit_cast<const char*>(&Files.compressedsize) + sizeof(uint32_t),
        std::back_inserter(TmpAll.tempbuffer));

    std::copy(std::bit_cast<const char*>(&Files.filepos),
        std::bit_cast<const char*>(&Files.filepos) + sizeof(uint32_t),
        std::back_inserter(TmpAll.tempbuffer));

    std::copy(Files.emptybytes.begin(),
        Files.emptybytes.end(),
        std::back_inserter(TmpAll.tempbuffer));
}