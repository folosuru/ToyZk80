
set(CMAKE_C_STANDARD 11)

add_executable(ToyTk80_asm main.c
        assembler.c
        assembly.c
        disassembler.c
        assembler.h
        instructions.c
        instructions.h
        text_util.c
        main.c
)