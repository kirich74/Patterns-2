cmake_minimum_required(VERSION 3.9)
project(patterns)

set(CMAKE_CXX_STANDARD 17)

add_executable(patterns main.cpp typelist.h decompressors.h compressedTypes.h)