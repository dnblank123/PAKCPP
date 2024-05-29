#include "filestruct.hpp"
#include "compress.hpp"

#include "src/zopfli/zlib_container.h"

namespace Compress {
    void CompFile(ContentFiles& Files, unsigned int& threads) {
        ZopfliOptions options;
        ZopfliInitOptions(&options);
        options.numiterations = 100;
        options.findminimumrec = 20;
        options.numthreads = threads;
        options.blocksplittingmax = 0;
        options.mode = 0x0004;
        ZopfliPredefinedSplits splits;
        splits.splitpoints = 0;
        splits.npoints = 0;

        ZopfliZlibCompress(&options, Files.incompress.data(), Files.filesize, 
            &Files.compressedbuffer, &Files.compressedsize, &splits);

    }

    void CompReset(ContentFiles& Files) {
        Files.incompress.clear();
        Files.compressedsize = 0;
        Files.compressedbuffer = nullptr;
        Files.filesize = 0;
        Files.filepos = 0;
    }
}