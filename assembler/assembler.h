#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "instructions.h"

typedef struct BufferArea_ {
    char* buffer;
    int size;
} BufferArea;

struct seeking_text {
    const char* const start;
    const char* current;
};

struct unresolved_label {
    const char* name;
    const char* name_len;
};

const char* seek_instruction(struct seeking_text* current, int* const len_out);
BufferArea assemble(char* source);
ASM_Opcode *tryCreateOperand(const char* name, int name_len,
                             struct seeking_text* text, const struct ASM_Instruction* instruction);
ASM_Opcode* findInstruction(struct seeking_text* text);
#endif  // ASSEMBLER_H
