
set(CMAKE_C_STANDARD 11)

add_executable(ToyZk80_asm main.c
        assembler.c
        assembly.c
        disassembler.c
        assembler.h
        main.c
)
target_link_libraries(ToyZk80_asm PRIVATE ToyZk80_util ToyZk80_instruction)
target_include_directories(ToyZk80_asm PRIVATE ../)