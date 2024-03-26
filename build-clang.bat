clang++ -fdata-sections -ffunction-sections src\main.cpp src\utils\compress.cpp src\utils\filesys.cpp -Ofast -mavx -Izopfli\ -Isrc\utils\ -std=c++20 -Llib -lmcfgthread -lzopfli -o pakcpp.exe -Wl,--gc-sections

REM -fdata-sections -ffunction-sections test.cpp -o test.exe -Wl,--gc-sections