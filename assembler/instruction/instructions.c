#include "instructions.h"
// ref: http://www.ze.em-net.ne.jp/~kenken/misc/z80.html
struct ASM_Instruction ASM_instructions[] = {
#include "./instructions.txt"
};

int ASM_Instructions_count = sizeof(ASM_instructions) / sizeof(ASM_instructions[0]);
