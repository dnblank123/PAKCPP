#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <cstdint>

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
