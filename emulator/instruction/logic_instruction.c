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
        : "+a"(Context_instance.A), "=r"(Context_instance.SF), "=r"(Context_instance.ZF), \
          "=r"(popcnt_result)                                                                         \
        : "r"(operand));                                                                              \
    Context_instance.HC = 1;                                                                    \
    Context_instance.PV = popcnt_result & 1;                                                        \
    Context_instance.NF = 0;                                                                    \
    Context_instance.CF = 0;

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
        : "+a"(Context_instance.A), "=r"(Context_instance.SF), "=r"(Context_instance.ZF), \
          "=r"(popcnt_result)                                                                         \
        : "r"(operand));                                                                            \
    Context_instance.HC = 1;                                                                    \
    Context_instance.PV = popcnt_result;                                                        \
    Context_instance.NF = 0;                                                                    \
    Context_instance.CF = 0;

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
        : "+r"(Context_instance.A), "=r"(Context_instance.SF), "=r"(Context_instance.ZF), \
          "=r"(popcnt_result)                                                                         \
        : "r"(operand));                                                                   \
    Context_instance.HC = 1;                                                                    \
    Context_instance.PV = popcnt_result;                                                        \
    Context_instance.NF = 0;                                                                    \
    Context_instance.CF = 0;

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


// Rotate Right Circular
void instruction_RRCA() {
    Context_instance.CF = Context_instance.A & 0x1;
    asm(
        "ror %0, 1":
        "+r"(Context_instance.A):);
    Context_instance.PC++;
}

// Rotate Left Circular
void instruction_RLCA() {
    Context_instance.CF = (Context_instance.A >> 7) & 0x1;
    asm(
        "rol %0, 1":
        "+r"(Context_instance.A):);
    Context_instance.PC++;
}

// Rotate Right Circular with Carry
void instruction_RRA() {
    uint8_t CF_old = Context_instance.CF;
    Context_instance.CF = Context_instance.A  & 0x1;

    // set 7 bit to carry
    // before:    7 6 5 4 3 2 1 0
    // carry_set: 7 6 5 4 3 2 1 C
    // after rol: C 7 6 5 4 3 2 1
    Context_instance.A = ((Context_instance.A) & 0xFE) | CF_old;

    asm(
        "ror %0, 1":
        "+r"(Context_instance.A):);
    Context_instance.PC++;
}

// Rotate Left Circular with Carry
void instruction_RLA() {
    uint8_t CF_old = Context_instance.CF;
    Context_instance.CF = (Context_instance.A >> 7) & 0x1;

    // set 7 bit to carry
    // carry_set: C 6 5 4 3 2 1 0
    // after rol: 6 5 4 3 2 1 0 C
    Context_instance.A = ((Context_instance.A) & 0x7A) | (CF_old << 7);

    asm(
        "rol %0, 1":
        "+r"(Context_instance.A):);
    Context_instance.PC++;
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
    check_test(Context_instance.SF == 0);
    check_test(Context_instance.ZF == 0);
    check_test(Context_instance.HC == 1);
    check_test(Context_instance.PV == 0);
    check_test(Context_instance.NF == 0);
    check_test(Context_instance.CF == 0);
    check_PC_test(1);

    Context_instance.A = 0xAA;  // 1010 1010
    Context_instance.B = 0xB8;  // 1011 1000
    // 0xA8 = 1010 1000
    check_test_start(instruction_AND_B(), Context_instance.A == 0xA8);
    check_test(Context_instance.SF == 1);
    check_test(Context_instance.ZF == 0);
    check_test(Context_instance.HC == 1);
    check_test(Context_instance.PV == 1);
    check_test(Context_instance.NF == 0);
    check_test(Context_instance.CF == 0);

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


    Context_instance.A = 0x80; // 1000 0000
    check_test_start(instruction_RRCA(), Context_instance.A == 0x40); // 0100 0000
    check_test(Context_instance.CF == 0);

    Context_instance.A = 0x1; // 0000 0001
    check_test_start(instruction_RRCA(), Context_instance.A == 0x80); // 1000 0000
    check_test(Context_instance.CF == 1);

    Context_instance.A = 0x40; // 0100 0000
    check_test_start(instruction_RLCA(), Context_instance.A == 0x80); // 1000 0000
    check_test(Context_instance.CF == 0);

    Context_instance.A = 0x80; // 1000 0000
    check_test_start(instruction_RLCA(), Context_instance.A == 0x1); // 0000 0001
    check_test(Context_instance.CF == 1);

    Context_instance.A = 0x4A; // 0100 1010
    Context_instance.CF = 1;
    check_test_start(instruction_RLA(), Context_instance.A == 0x95); // 1001 0101
    check_test(Context_instance.CF == 0); // 0

    Context_instance.A = 0x4A; // 0100 1010
    Context_instance.CF = 0;
    check_test_start(instruction_RLA(), Context_instance.A == 0x94); // 1001 0100
    check_test(Context_instance.CF == 0); // 0

    Context_instance.A = 0x4A; // 0100 1010
    Context_instance.CF = 1;
    check_test_start(instruction_RRA(), Context_instance.A == 0xA5); // 1010 0101
    check_test(Context_instance.CF == 0);

    Context_instance.A = 0xB7; // 1011 0111
    Context_instance.CF = 0;
    check_test_start(instruction_RRA(), Context_instance.A == 0x5B); // 0101 1011
    check_test(Context_instance.CF == 1);
}
#endif
