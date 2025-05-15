cmake_minimum_required(VERSION 3.31)
project(ToyTk80 C)

set(CMAKE_C_STANDARD 11)

add_executable(ToyTk80 main.c
        assembler/assembler.c
        assembler/assembler.h
        assembler/instructions.c
        assembler/instructions.h)
