#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "filestruct.hpp"
#include "filesys.hpp"

int main(int argc, char** argv) {
    fs::FileStream fstrm;
    ContentFiles cofi;
    Header head;
    TempAlloc tmpall;

    std::filesystem::path argvpath;
    {
        std::ofstream{"00Resource.pak"};
    }

    if (argc == 1 || argc > 2) {
        std::cout << "Usage: pakcpp.exe folder";
        std::quick_exit(0);
    }

    argvpath = argv[1];

    if (std::filesystem::exists("00Resource.pak")) {
        std::filesystem::remove("00Resource.pak");
    }

    fstrm.File(argvpath, cofi, head, tmpall);

    return 0;
}
