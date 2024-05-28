cmake_minimum_required(VERSION 3.10)

project(pakcpp VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

// set(INCLUDE_DIRS "C:\\Users\\Admin\\Desktop\\cmake-test\\libraries\\fmt\\include\\fmt")

// add_subdirectory(libraries/fmt)
add_executable(pakcpp src/main.cpp)
// target_include_directories(my_executable PRIVATE INCLUDE_DIRS)
// target_link_libraries(my_executable PRIVATE fmt::fmt)

