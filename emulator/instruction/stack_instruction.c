#include "stack_instruction.h"

#include "../emulator.h"
#include "instruction_test_util.h"

#define INSTRUCTION_PUSH(R1, R2)                                               \
    void instruction_PUSH_##R1##R2() {                                         \
        MemoryManager_ByteWrite(Context_instance.SP - 1, Context_instance.R1); \
        MemoryManager_ByteWrite(Context_instance.SP - 2, Context_instance.R2); \
        Context_instance.SP -= 2;                                              \
        Context_instance.PC++;                                                 \
    }

#define INSTRUCTION_POP(R1, R2)                                                \
    void instruction_POP_##R1##R2() {                                          \
        Context_instance.R2 = MemoryManager_ByteRead(Context_instance.SP);     \
        Context_instance.R1 = MemoryManager_ByteRead(Context_instance.SP + 1); \
        Context_instance.SP += 2;                                              \
        Context_instance.PC++;                                                 \
    }

#define DEF_FUNCTIONS(R1, R2) \
    INSTRUCTION_PUSH(R1, R2); \
    INSTRUCTION_POP(R1, R2)

DEF_FUNCTIONS(B, C)
DEF_FUNCTIONS(D, E)
DEF_FUNCTIONS(H, L)
DEF_FUNCTIONS(A, F)

#ifdef DEBUG
void test_instruction_stack() {
    Context_instance.BC = 0x1526;
    Context_instance.SP = 0x8020;
    check_test_start(instruction_PUSH_BC(), Context_instance.SP == 0x8020 - 2);
    check_PC_test(1);

    Context_instance.DE = 0x5432;
    check_test_start(instruction_POP_DE(), Context_instance.DE == 0x1526);
    check_test(Context_instance.SP == 0x8020);
    check_PC_test(1);
}
#endif
