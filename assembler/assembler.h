#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <stdbool.h>
#include <stdio.h>

#include "instructions.h"

typedef struct BufferArea_ {
    unsigned char* buffer;
    int size;
} BufferArea;

struct seeking_text {
    const char* const start;
    const char* current;
};

struct unresolved_label {
    const char* name;
    int name_len;
    unsigned char* write_position;
    unsigned char* next_instruction;

    enum {
        unresolved_label_type_relative,
        unresolved_label_type_absolute
    } type;
};

struct defined_label {
    const char* name;
    int name_len;
    const unsigned char* position;
};

struct Command_flags {
    FILE* out;
    int start_address;
    bool create_addressJump_label;
    bool show_assemble_address;
};

BufferArea assemble(const char* source, struct Command_flags* flag);
void disassemble(const char* source, int source_len, struct Command_flags* flag);

int getOpcodeSize(const struct ASM_Instruction* instruction);
void print_BufferArea(BufferArea area, FILE* out);
_Bool skip_whitespace(struct seeking_text* text);
_Bool skip_return(struct seeking_text* text);
int resolveLabel(struct unresolved_label* find_label, struct defined_label (*definedLabel)[], int definedLabel_count);

#endif  // ASSEMBLER_H
