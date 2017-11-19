cmake_minimum_required(VERSION 3.8)
project(xor)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Xor.h)
add_executable(xor ${SOURCE_FILES})