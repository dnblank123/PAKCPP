cmake_minimum_required(VERSION 3.10)

project(pakcpp)

include_directories(zopfli)

set(CMAKE_CXX_STANDARD 20)

#build other cmake
#link library
add_executable(pakcpp src/main.cpp)