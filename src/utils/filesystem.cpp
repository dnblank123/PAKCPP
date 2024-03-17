
#include <bit>
#include <cstdint>
#include <filesystem>
#include <fstream>

#include "filestruct.h"
#include "filesystem.h"

void FileSys::GetFiles(std::filesystem::path& FileLocation)
{
    ContentFiles contentfiles;

    const std::string& rootpath = "";//FileLoc.string();
    std::ifstream stream;
    std::ofstream file("00Resource.pak", std::ios::binary | std::ios::out | std::ios::app);

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ FileLocation }) {
        if (dir_entry.path().has_extension()) {

            const std::filesystem::path& filepath = dir_entry.path();
            std::string path = filepath.string();
            if (path.find(rootpath) == 0) {
                path.erase(0, rootpath.length());
            }

            stream.open(filepath, std::ios::binary);
            stream.seekg(0, std::ios::end);
            contentfiles.filesize = static_cast<size_t>(stream.tellg());
            stream.seekg(0, std::ios::beg);

            contentfiles.pathemptybytes.resize(256 - path.length());
            stream.read(std::bit_cast<char*>(contentfiles.incompress.data()), static_cast<std::uint32_t>(contentfiles.filesize));

            contentfiles.filepos = static_cast<size_t>(file.tellp());

        }
        stream.close();
    }
}