#include <stdarg.h>
#include <string.h>

#include "assembler.h"
#include "instructions.h"

int getOpcodeSize(const struct ASM_Instruction* instruction) {
    if (instruction == NULL) {
        return 1;
    }

    int result = (int)instruction->OpCodeCount;

    for (int i=0; i < sizeof(instruction->data) / sizeof(instruction->data[0]) ; i++) {
        switch (instruction->data[i]) {
            case data_none:
                return result;
            case data_n:
            case data_e:
            case data_d:
                result += 1;
                break;

            case data_nn:
                result += 2;
                break;
        }
    }
    return result;
}


int resolveLabel(struct unresolved_label* find_label, struct defined_label (*definedLabel)[], int definedLabel_count) {
    for (int i=0; i < definedLabel_count; i++) {
        if (find_label->name_len == (*definedLabel)[i].name_len &&
            strncmp(find_label->name, (*definedLabel)[i].name, (*definedLabel)[i].name_len) == 0){
            return i;
        }
    }
    return -1;
}