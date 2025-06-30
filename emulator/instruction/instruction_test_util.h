#ifndef TOYTK80_INSTRUCTIO_TEST_UTIL_H
#define TOYTK80_INSTRUCTIO_TEST_UTIL_H

#ifdef DEBUG
#include <stdio.h>
extern const char* last_test_instruction;

#define check_test_start(instruction, debug_stm)                                                \
    do {                                                                                        \
        Context_instance.PC = 0x8000; \
        instruction;                                                                            \
        last_test_instruction = #instruction;                                                   \
        if (!(debug_stm)) {                                                                     \
            fprintf(stderr, "test: " #instruction " is fail.\n    " #debug_stm " is false.\n"); \
        }                                                                                       \
    } while (0)

#define check_test(debug_stm)                                                                            \
    do {                                                                                                 \
        if (!(debug_stm)) {                                                                              \
            fprintf(stderr, "test: %s is fail.\n    " #debug_stm " is false.\n", last_test_instruction); \
        }                                                                                                \
    } while (0)

#define check_PC_test(instruction_size)                                                                            \
    do {                                                                                                 \
        if (Context_instance.PC != 0x8000 + instruction_size) {                                                                              \
            fprintf(stderr, "test: %s is fail.\n" \
                    "      Except opcode size is " #instruction_size ", but %d.\n\n",     \
                    last_test_instruction, Context_instance.PC - 0x8000); \
        }                                                                                                \
    } while (0)
#endif  // DEBUG

#endif  // TOYTK80_INSTRUCTIO_TEST_UTIL_H
