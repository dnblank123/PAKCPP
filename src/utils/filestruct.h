#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <cstdint>

inline constexpr auto HEADER_EMPTY_BYTES = 1024;
inline constexpr auto CONTENT_EMPTY_BYTES = 44;

struct Header {
    std::string filesignature = "EyedentityGames Packing File 0.1";
    size_t filecount = 0;
    static constexpr uint8_t unknown = 11;
    std::array<const char, HEADER_EMPTY_BYTES> headerbytes{};
    size_t fileindex = 0;
};

struct ContentFiles {
    std::vector<unsigned char> incompress{};
    std::size_t compressedsize = 0;
    unsigned char* compressedbuffer = nullptr;
    std::vector<char> pathemptybytes{};
    std::size_t filepos = 0;
    std::size_t filesize = 0;
    std::array<uint8_t, CONTENT_EMPTY_BYTES> emptybytes{};
};

struct TempAlloc {
    std::vector<char> tempbuffer{};
};