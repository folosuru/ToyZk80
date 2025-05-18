#include <stdio.h>
#include "assembler/assembler.h"
int main(void)
{
    const char* text = "LD (  HL )  , L\n";
    struct seeking_text st = {text, text};
    ASM_Opcode* op = findInstruction(&st);
    printf("%p\n", op);
    if (op!=NULL) {
        printf("%s %s\n", op->instruction->instruction, op->instruction->operand);
    }
}