# Dragon Nest ZPAK
Compressed PAK using [Zopfli](https://github.com/MrKrzYch00/zopfli).

## Requirements
- [CMake 3.10 or newer](https://cmake.org/)
- [GCC/Clang](https://winlibs.com/)

## Build instruction for GCC/Clang
```
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make or make
```

## TODO
- [ ] Rewrite for better code
- ~Use ZSTD for more high ratio compression~ Different Headers
- [ ] GUI (ImGUI/ImWindow) for practice purposes.
- [ ] Fuzzing and Tests
- [x] CMake tool

## Special Thanks
- Vahr for the File Format
> https://vincentzhang96.github.io/DragonNestFileFormats/files/pak/
