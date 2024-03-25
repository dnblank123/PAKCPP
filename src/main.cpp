#include <cstdlib>
#include <filesystem>

#include <iostream>

#include "filesys.h"
#include "filestruct.h"
#include "compress.h"

int main(int argc, char** argv) {
    FileStream fstrm;
    ContentFiles cofi;
    Header head;

    std::filesystem::path argvpath;

#ifdef NDEBUG
    if (argc == 1 || argc > 2) {
        std::cout << "Usage: pakcpp.exe folder";
        std::quick_exit(0);
    }

    argvpath = argv[1];
#else 
    argvpath = "test";
#endif

    if (std::filesystem::exists("00Resource.pak")) {
        std::filesystem::remove("00Resource.pak");
    }

    fstrm.File(argvpath, cofi, head);
    
    fstrm.WriteHeadFile(head);

    //Compress::CompFile(cofi);

    return 0;
}
