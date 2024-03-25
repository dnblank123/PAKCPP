#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>
#include <iostream>

#include "filestruct.h"
#include "filesys.h"

inline constexpr auto PATH_BYTES_RESIZE = 256;

void FileStream::File(const std::filesystem::path& FileLocation, ContentFiles& Files, Header& Head) {
    const std::string rootpath = FileLocation.string();

    for (auto const &dir_entry : std::filesystem::recursive_directory_iterator{ FileLocation }) {
        if (dir_entry.path().has_extension()) {
            Head.filecount++;
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

        }
        InFile.close();
    }
}

void FileStream::WriteHeadFile(Header& Head) {
    OutFile.open("00Resource.pak", std::ios_base::binary | std::ios_base::in);
    OutFile.write(Head.headerbytes.data(), Head.headerbytes.size());
    OutFile.seekp(0);
    OutFile.write(Head.filesignature, strlen(Head.filesignature));
    OutFile.seekp(256);
    OutFile.write(std::bit_cast<char*>(&Head.unknown), sizeof(Head.unknown));
    OutFile.seekp(260);
    OutFile.write(std::bit_cast<char*>(&Head.filecount), sizeof(static_cast<uint32_t>(Head.filecount)));
    OutFile.seekp(264);
    OutFile.write(std::bit_cast<char*>(&Head.fileindex), sizeof(static_cast<uint32_t>(Head.fileindex)));
    OutFile.close();
}