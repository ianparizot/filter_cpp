cmake_minimum_required(VERSION 3.31)
project(filter CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(filter src/main.cpp 
        src/functions.h 
        src/functions.cpp 
        src/bmp.h
)        