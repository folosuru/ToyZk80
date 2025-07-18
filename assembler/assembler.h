#ifndef TOYZK80ASM_ASSEMBLER_H
#define TOYZK80ASM_ASSEMBLER_H
#include <stdbool.h>
#include <stdio.h>

#include "instruction/instructions.h"
#include "util/util.h"

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

int resolveLabel(struct unresolved_label* find_label, struct defined_label (*definedLabel)[], int definedLabel_count);

#endif  // TOYZK80ASM_ASSEMBLER_H
