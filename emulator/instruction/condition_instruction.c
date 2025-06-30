#include "condition_instruction.h"
#include "../emulator.h"

void INSTRUCTION_CP_PROCESS(Byte operand) {
    __asm__(                                                                                          \
        "cmp %0, %5\n\t"                                                                              \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "seto %3\n\t"                                                                                 \
        "setc %4\n\t"                                                                                 \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
        : "r"(operand));                                                                              \
    Context_instance.flags.NF = 0;
}

#define INSTRUCTION_CP_R(R)  \
    void instruction_CP_##R() { \
        INSTRUCTION_CP_PROCESS(Context_instance.R); \
        Context_instance.PC++;\
    }

void instruction_CP_HLp() {
    INSTRUCTION_CP_PROCESS(MemoryManager_ByteRead(Context_instance.HL));
    Context_instance.PC++;
}
void instruction_CP_n() {
    INSTRUCTION_CP_PROCESS(MemoryManager_ByteRead(Context_instance.PC+1));
    Context_instance.PC+=2;
}


#define DEFINE_CONDITION_INSTRUCTION(R)\
    INSTRUCTION_CP_R(R);     \

DEFINE_CONDITION_INSTRUCTION(B)
DEFINE_CONDITION_INSTRUCTION(C)
DEFINE_CONDITION_INSTRUCTION(D)
DEFINE_CONDITION_INSTRUCTION(E)
DEFINE_CONDITION_INSTRUCTION(H)
DEFINE_CONDITION_INSTRUCTION(L)
DEFINE_CONDITION_INSTRUCTION(A)

