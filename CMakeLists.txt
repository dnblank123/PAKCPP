cmake_minimum_required(VERSION 3.10)

project(pakcpp VERSION 1.0.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(UTILS_SOURCES 
    src/utils/compress.cpp
    src/utils/filesys.cpp
)

set(ZOPFLI_INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/zopfli"
)
set(UTILS_INCLUDE_DIRS 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils"
)
set(CMAKE_CXX_FLAGS "-O2 -mavx -s")
set(CMAKE_BUILD_TYPE Release)

include_directories(${ZOPFLI_INCLUDE_DIRS})
include_directories(${UTILS_INCLUDE_DIRS})

add_subdirectory(lib/zopfli) # call zopfli cmakelist
add_library(utils STATIC ${UTILS_SOURCES})

add_executable(pakcpp src/main.cpp)

target_include_directories(libzopfli PRIVATE ZOPFLI_INCLUDE_DIRS)
target_include_directories(utils PRIVATE UTILS_INCLUDE_DIRS)

target_link_libraries(utils PRIVATE libzopfli)
target_link_libraries(pakcpp PRIVATE utils)
target_link_libraries(pakcpp PRIVATE mcfgthread)

