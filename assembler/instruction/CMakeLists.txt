set(CMAKE_C_STANDARD 11)

add_library(ToyZk80_instruction
        instructions.c
        instructions.h
        assembly.c
        assembly.c
)

target_link_libraries(ToyZk80_instruction PRIVATE ToyZk80_util)
target_include_directories(ToyZk80_instruction PRIVATE ../../)