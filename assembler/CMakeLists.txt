
set(CMAKE_C_STANDARD 11)

add_executable(ToyTk80_asm main.c
        assembler.c
        assembly.c
        disassembler.c
        assembler.h
        instructions.c
        instructions.h
        main.c
)

target_link_libraries(ToyTk80_asm PRIVATE ToyTk80_util)
target_include_directories(ToyTk80_asm PRIVATE ../)