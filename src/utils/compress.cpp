#include "filestruct.h"
#include "compress.h"

#include "zopfli.h"

//filesystem receives the path
//filesystem.cpp iterate on the files, then calls Compress::File at main which receives a ?? then return a ??

void Compress::CompFile(ContentFiles& Files) {
    ZopfliOptions options;

    ZopfliInitOptions(&options);
    options.numiterations = 100;

    ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, Files.incompress.data(), 
        Files.filesize, &Files.compressedbuffer, &Files.compressedsize);
}

void Compress::CompReset(ContentFiles& Files) {
    Files.incompress.clear();
    Files.compressedsize = 0;
    Files.compressedbuffer = nullptr;
    Files.filesize = 0;
    Files.filepos = 0;
}