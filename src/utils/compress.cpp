
#include <vector>

#include "filestruct.h"
#include "compress.h"

#include "zopfli.h"

//filesystem receives the path
//filesystem.cpp iterate on the files, then calls Compress::File at main which receives a ?? then return a ??

void Compress::File(ContentFiles *files)
{
    ZopfliOptions options;

    ZopfliInitOptions(&options);
    options.numiterations = 100;

    ZopfliCompress(&options, ZOPFLI_FORMAT_ZLIB, files->incompress.data(), files->filesize, &files->compressedbuffer, &files->compressedsize);

    files->incompress.clear();
    files->compressedsize = 0;
    files->compressedbuffer = nullptr;
    files->filesize = 0;
    files->filepos = 0;
}
