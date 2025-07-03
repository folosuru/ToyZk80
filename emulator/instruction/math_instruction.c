#include "math_instruction.h"

#include "../emulator.h"
#include "instruction_test_util.h"

#define INSTRUCTION_INC_R(RR)                                                                              \
    void instruction_INC_##RR() {                                                                          \
        __asm__(                                                                                           \
            "inc %0\n\t"                                                                                   \
            "sets %1\n\t"                                                                                  \
            "setz %2\n\t"                                                                                  \
            "seto %3\n\t"                                                                                  \
            : "+r"(Context_instance.RR), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(Context_instance.flags.PV)                                                              \
            :);                                                                                            \
        Context_instance.flags.NF = 1;                                                                     \
        Context_instance.flags.CF = Context_instance.flags.ZF;                                             \
        Context_instance.PC++;                                                                             \
    }

#define INSTRUCTION_DEC_R(R)                                                                              \
    void instruction_DEC_##R() {                                                                          \
        Context_instance.flags.CF = 0;                                                                    \
        if (Context_instance.R == 0) Context_instance.flags.CF = 1;                                       \
        __asm__(                                                                                          \
            "dec %0\n\t"                                                                                  \
            "sets %1\n\t"                                                                                 \
            "setz %2\n\t"                                                                                 \
            "seto %3\n\t"                                                                                 \
            : "+r"(Context_instance.R), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(Context_instance.flags.PV)                                                             \
            :);                                                                                           \
        Context_instance.flags.NF = 0;                                                                    \
        Context_instance.PC++;                                                                            \
    }

#define INSTRUCTION_ADD_A_PROCESS(operand)                                                            \
    __asm__(                                                                                          \
        "add %0, %5\n\t"                                                                              \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "seto %3\n\t"                                                                                 \
        "setc %4\n\t"                                                                                 \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
        : "r"(operand));                                                                              \
    Context_instance.flags.NF = 1;

#define INSTRUCTION_ADD_A_R(add_register)                        \
    void instruction_ADD_A_##add_register() {                    \
        INSTRUCTION_ADD_A_PROCESS(Context_instance.add_register) \
        Context_instance.PC++;                                   \
    }

void instruction_ADD_A_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    INSTRUCTION_ADD_A_PROCESS(operand)
    Context_instance.PC += 2;
}

void instruction_ADD_A_HLp() {
    INSTRUCTION_ADD_A_PROCESS(MemoryManager_ByteRead(Context_instance.HL))
    Context_instance.flags.NF = 1;
}

#define INSTRUCTION_ADC_A_PROCESS(operand)                                                            \
    __asm__(                                                                                          \
        "adc %0, %5\n\t"                                                                              \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "seto %3\n\t"                                                                                 \
        "setc %4\n\t"                                                                                 \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
        : "r"(operand));                                                                              \
    Context_instance.flags.NF = 1;

#define INSTRUCTION_ADC_A_R(add_register)                        \
    void instruction_ADC_A_##add_register() {                    \
        INSTRUCTION_ADC_A_PROCESS(Context_instance.add_register) \
        Context_instance.PC++;                                   \
    }

void instruction_ADC_A_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    INSTRUCTION_ADC_A_PROCESS(operand)
    Context_instance.PC += 2;
}

void instruction_ADC_A_HLp() {
    Byte operand = MemoryManager_ByteRead(Context_instance.HL);
    INSTRUCTION_ADC_A_PROCESS(operand)
    Context_instance.PC += 1;
}

void INSTRUCTION_SUB_A_PROCESS(Byte operand) {
    __asm__(
        "sub %0, %5\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        "setc %4\n\t"
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)
        : "r"(operand));
    Context_instance.flags.NF = 0;
}

#define INSTRUCTION_SUB_A_R(sub_register)                         \
    void instruction_SUB_##sub_register() {                       \
        INSTRUCTION_SUB_A_PROCESS(Context_instance.sub_register); \
        Context_instance.PC++;                                    \
    }

void instruction_SUB_HLp() {
    Byte operand = MemoryManager_ByteRead(Context_instance.HL);
    INSTRUCTION_SUB_A_PROCESS(operand);
    Context_instance.PC++;
}

void instruction_SUB_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    INSTRUCTION_SUB_A_PROCESS(operand);
    Context_instance.PC += 2;
}

#define INSTRUCTION_SBC_A_PROCESS(operand)                                                            \
    __asm__(                                                                                          \
        "sbb %0, %5\n\t"                                                                              \
        "sets %1\n\t"                                                                                 \
        "setz %2\n\t"                                                                                 \
        "seto %3\n\t"                                                                                 \
        "setc %4\n\t"                                                                                 \
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
        : "r"(operand));                                                                              \
    Context_instance.flags.NF = 0;

#define INSTRUCTION_SBC_A_R(sub_register)                        \
    void instruction_SBC_##sub_register() {                      \
        INSTRUCTION_SBC_A_PROCESS(Context_instance.sub_register) \
        Context_instance.PC += 1;                                \
    }

void instruction_SBC_A_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    INSTRUCTION_SBC_A_PROCESS(operand)
    Context_instance.PC += 2;
}

void instruction_SBC_HLp() {
    Byte operand = MemoryManager_ByteRead(Context_instance.HL);
    INSTRUCTION_SBC_A_PROCESS(operand)
    Context_instance.PC += 1;
}

#define DEFINE_MATH_INSTRUCTION(R) \
    INSTRUCTION_INC_R(R);          \
    INSTRUCTION_DEC_R(R);          \
    INSTRUCTION_ADD_A_R(R);        \
    INSTRUCTION_ADC_A_R(R);        \
    INSTRUCTION_SUB_A_R(R);        \
    INSTRUCTION_SBC_A_R(R);

DEFINE_MATH_INSTRUCTION(B)
DEFINE_MATH_INSTRUCTION(C)
DEFINE_MATH_INSTRUCTION(D)
DEFINE_MATH_INSTRUCTION(E)
DEFINE_MATH_INSTRUCTION(H)
DEFINE_MATH_INSTRUCTION(L)
DEFINE_MATH_INSTRUCTION(A)

void instruction_INC_HLp() {
    Byte result = MemoryManager_ByteRead(Context_instance.HL);
    __asm__(
        "inc %0\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        : "+r"(result), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV)
        :);
    MemoryManager_ByteWrite(Context_instance.HL, result);
    Context_instance.flags.NF = 1;
    Context_instance.PC++;
}

void instruction_DEC_HLp() {
    Byte result = MemoryManager_ByteRead(Context_instance.HL);
    __asm__(
        "dec %0\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        : "+r"(result), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV)
        :);
    MemoryManager_ByteWrite(Context_instance.HL, result);
    Context_instance.flags.NF = 0;
    Context_instance.PC++;
}

#define INSTRUCTION_ADD_HL_RR(R)                                         \
    void instruction_ADD_HL_##R() {                                      \
        __asm__(                                                         \
            "add %0, %2\n\t"                                             \
            "setc %1\n\t"                                                \
            : "+r"(Context_instance.HL), "=r"(Context_instance.flags.CF) \
            : "r"(Context_instance.R));                                  \
        Context_instance.flags.NF = 1;                                   \
    }

#define DEFINE_MATH_INSTRUCTION_RR(R) INSTRUCTION_INC_R(R) INSTRUCTION_DEC_R(R) INSTRUCTION_ADD_HL_RR(R)

DEFINE_MATH_INSTRUCTION_RR(BC)
DEFINE_MATH_INSTRUCTION_RR(DE)
DEFINE_MATH_INSTRUCTION_RR(HL)
DEFINE_MATH_INSTRUCTION_RR(SP)

#ifdef DEBUG
void test_instruction_math() {
    // ADD test
    // ADD A, B
    Context_instance.A = 250;
    Context_instance.B = 10;
    check_test_start(instruction_ADD_A_B(), Context_instance.A == 0x4);
    check_test(Context_instance.flags.SF == 0);
    check_test(Context_instance.flags.ZF == 0);
    // Half Carry flag is not implement
    check_test(Context_instance.flags.PV == 0);
    check_test(Context_instance.flags.NF == 1);
    check_test(Context_instance.flags.CF == 1);
    check_PC_test(1);

    // 0x64 + 0x32 = 0x96
    // 100 + 50 = 150 (unsigned)
    // 100 + 50 = -106(signed)
    Context_instance.A = 0x64;
    Context_instance.B = 0x32;
    check_test_start(instruction_ADD_A_B(), Context_instance.A == 0x96);
    check_test(Context_instance.flags.SF == 1);
    check_test(Context_instance.flags.ZF == 0);
    // Half Carry flag is not implement
    check_test(Context_instance.flags.PV == 1);
    check_test(Context_instance.flags.NF == 1);
    check_test(Context_instance.flags.CF == 0);

    // SUB test
    // SUB B
    Context_instance.A = 0x30;
    Context_instance.B = 0x20;
    check_test_start(instruction_SUB_B(), Context_instance.A == 0x10);
    check_test(Context_instance.flags.NF == 0);
    check_PC_test(1);

    // INC test
    // INC B
    Context_instance.B = 0x10;
    check_test_start(instruction_INC_B(), Context_instance.B == 0x11);
    check_test(Context_instance.flags.SF == 0);
    check_test(Context_instance.flags.ZF == 0);
    // Half Carry flag is not implement
    check_test(Context_instance.flags.PV == 0);
    check_test(Context_instance.flags.NF == 1);
    check_test(Context_instance.flags.CF == 0);
    check_PC_test(1);

    Context_instance.B = 0xFF;
    check_test_start(instruction_INC_B(), Context_instance.B == 0x0);
    check_test(Context_instance.flags.SF == 0);
    check_test(Context_instance.flags.ZF == 1);
    // Half Carry flag is not implement
    check_test(Context_instance.flags.PV == 0);
    check_test(Context_instance.flags.NF == 1);
    check_test(Context_instance.flags.CF == 1);

    // DEC test
    // DEC B
    Context_instance.B = 0x0;
    check_test_start(instruction_DEC_B(), Context_instance.B == 0xFF);
    check_test(Context_instance.flags.SF == 1);
    check_test(Context_instance.flags.ZF == 0);
    // Half Carry flag is not implement
    check_test(Context_instance.flags.PV == 0);
    check_test(Context_instance.flags.NF == 0);
    check_test(Context_instance.flags.CF == 1);
    check_PC_test(1);

    // TODO: add adc, abc test
}

#endif
