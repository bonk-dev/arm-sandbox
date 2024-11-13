cmake_minimum_required(VERSION 3.29)
project(arm_sandbox)

set(CMAKE_CXX_STANDARD 20)

add_executable(arm_sandbox main.cpp
        disassembly/A64Decoder.cpp
        disassembly/A64Decoder.h
)
