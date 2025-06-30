#include "logic_instruction.h"

#include "../emulator.h"
#include "instruction_test_util.h"

#define INSTRUCTION_AND_PROCESS(operand)                                                              \
    uint16_t popcnt_result;                                                                           \
    __asm__(                                                                                          \
        "and %0, %4\n\t"                                                                              \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "mov ah, 0\n\t"                                                                               \
        "popcnt %3, ax\n\t" /* for before Nehalem gen user: sorry.*/                                  \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(popcnt_result)                                                                         \
        : "r"(operand));                                                                              \
    Context_instance.flags.HC = 1;                                                                    \
    Context_instance.flags.PV = popcnt_result;                                                        \
    Context_instance.flags.NF = 0;                                                                    \
    Context_instance.flags.CF = 0;

#define INSTRUCTION_AND_R(R)                        \
    void instruction_AND_##R() {                    \
        INSTRUCTION_AND_PROCESS(Context_instance.R) \
        Context_instance.PC++;                      \
    }

void instruction_AND_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    INSTRUCTION_AND_PROCESS(operand)
    Context_instance.PC += 2;
}
void instruction_AND_HLp() {
    Byte operand = MemoryManager_ByteRead(Context_instance.HL);
    INSTRUCTION_AND_PROCESS(operand)
    Context_instance.PC++;
}

#define INSTRUCTION_OR_PROCESS(operand) \
    uint16_t popcnt_result;                                                                           \
    __asm__(                                                                                          \
        "or %0, %4\n\t"                                                                               \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "mov ah, 0\n\t"                                                                               \
        "popcnt %3, ax\n\t" /* for before Nehalem gen user: sorry.*/                                  \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(popcnt_result)                                                                         \
        : "r"(operand));                                                                            \
    Context_instance.flags.HC = 1;                                                                    \
    Context_instance.flags.PV = popcnt_result;                                                        \
    Context_instance.flags.NF = 0;                                                                    \
    Context_instance.flags.CF = 0;

#define INSTRUCTION_OR_R(R)                                                                               \
    void instruction_OR_##R() {                                                                       \
        INSTRUCTION_OR_PROCESS(Context_instance.R)                                                                                                  \
        Context_instance.PC++;                                                                            \
    }
void instruction_OR_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    INSTRUCTION_OR_PROCESS(operand)
    Context_instance.PC += 2;
}
void instruction_OR_HLp() {
    Byte operand = MemoryManager_ByteRead(Context_instance.HL);
    INSTRUCTION_OR_PROCESS(operand)
    Context_instance.PC++;
}

#define INSTRUCTION_XOR_PROCESS(operand) \
    uint16_t popcnt_result;                                                                           \
    __asm__(                                                                                          \
        "xor %0, %4\n\t"                                                                              \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "mov ah, 0\n\t"                                                                               \
        "popcnt %3, ax\n\t" /* for before Nehalem gen user: sorry.*/                                  \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(popcnt_result)                                                                         \
        : "r"(operand));                                                                   \
    Context_instance.flags.HC = 1;                                                                    \
    Context_instance.flags.PV = popcnt_result;                                                        \
    Context_instance.flags.NF = 0;                                                                    \
    Context_instance.flags.CF = 0;

#define INSTRUCTION_XOR_R(R)                                                                              \
    void instruction_XOR_##R() {                                                                          \
        INSTRUCTION_XOR_PROCESS(Context_instance.R)                                                                           \
        Context_instance.PC++;                                                                            \
    }
void instruction_XOR_n() {
    INSTRUCTION_XOR_PROCESS(MemoryManager_ByteRead(Context_instance.PC + 1));
    Context_instance.PC += 2;
}
void instruction_XOR_HLp() {
    INSTRUCTION_XOR_PROCESS(MemoryManager_ByteRead(Context_instance.HL));
    Context_instance.PC += 1;
}

#define DEFINE_LOGIC_INSTRUCTION(R) \
    INSTRUCTION_AND_R(R);           \
    INSTRUCTION_OR_R(R);            \
    INSTRUCTION_XOR_R(R);

DEFINE_LOGIC_INSTRUCTION(B)
DEFINE_LOGIC_INSTRUCTION(C)
DEFINE_LOGIC_INSTRUCTION(D)
DEFINE_LOGIC_INSTRUCTION(E)
DEFINE_LOGIC_INSTRUCTION(H)
DEFINE_LOGIC_INSTRUCTION(L)
DEFINE_LOGIC_INSTRUCTION(A)

#ifdef DEBUG
void test_instruction_logic() {
    Context_instance.A = 0xAA;  // 1010 1010
    Context_instance.B = 0x33;  // 0011 0011
    // 0x22 = 0010 0010
    check_test_start(instruction_AND_B(), Context_instance.A == 0x22);
    check_test(Context_instance.flags.SF == 0);
    check_test(Context_instance.flags.ZF == 0);
    check_test(Context_instance.flags.HC == 1);
    check_test(Context_instance.flags.PV == 0);
    check_test(Context_instance.flags.NF == 0);
    check_test(Context_instance.flags.CF == 0);
    check_PC_test(1);

    Context_instance.A = 0xAA;  // 1010 1010
    Context_instance.B = 0xB8;  // 1011 1000
    // 0xA8 = 1010 1000
    check_test_start(instruction_AND_B(), Context_instance.A == 0xA8);
    check_test(Context_instance.flags.SF == 1);
    check_test(Context_instance.flags.ZF == 0);
    check_test(Context_instance.flags.HC == 1);
    check_test(Context_instance.flags.PV == 1);
    check_test(Context_instance.flags.NF == 0);
    check_test(Context_instance.flags.CF == 0);

    Context_instance.A = 0xAA;  // 1010 1010
    MemoryManager_ByteWrite(0x8000+1, 0x33);  // 0011 0011
    // 0x22 = 0010 0010
    check_test_start(instruction_AND_n(), Context_instance.A == 0x22);
    check_PC_test(2);

    Context_instance.A = 0xAA;  // 1010 1010
    Context_instance.HL = 0x8080;
    MemoryManager_ByteWrite(0x8080, 0x33);  // 0011 0011
    // 0x22 = 0010 0010
    check_test_start(instruction_AND_HLp(), Context_instance.A == 0x22);
    check_PC_test(1);

}
#endif
