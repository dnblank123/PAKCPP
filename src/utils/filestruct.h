#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

inline constexpr auto HEADER_EMPTY_BYTES = 1024;
inline constexpr auto CONTENT_EMPTY_BYTES = 44;

struct Header {
    std::array<const char, HEADER_EMPTY_BYTES> headerbytes{};
    const char* filesignature = "EyedentityGames Packing File 0.1";
    std::uint32_t filecount = 0;
    std::uint32_t fileindex = 0;
    static constexpr std::uint8_t unknown = 11;
};

struct ContentFiles {
    std::vector<unsigned char> incompress{};
    std::size_t compressedsize = 0;
    unsigned char* compressedbuffer = nullptr;
    std::vector<char> pathemptybytes{};
    std::uint32_t filepos = 0;
    std::uint32_t filesize = 0;
    std::array<std::uint8_t, CONTENT_EMPTY_BYTES> emptybytes{};
};

struct TempAlloc {
    std::vector<char> tempbuffer{};
};