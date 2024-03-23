#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>

#include "filestruct.h"
#include "filesys.h"

inline constexpr auto PATH_BYTES_RESIZE = 256;

void FileStream::File(const std::filesystem::path &FileLocation, ContentFiles &Files) {
    const std::string rootpath = FileLocation.string();

    OutFile.open("00Resource.pak", std::ios::binary | std::ios::out | std::ios::app);

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
            InFile.read(std::bit_cast<char*>(Files.incompress.data()), static_cast<std::uint32_t>(Files.filesize));

            Files.filepos = static_cast<std::uint32_t>(OutFile.tellp());

        }
        InFile.close();
    }
}
