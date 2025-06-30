#include "ld_instruction.h"

#include "../emulator.h"
#include "instruction_test_util.h"

#define INSTRUCTION_LD_R_R(left, right)                 \
    void instruction_LD_##left##_##right() {            \
        Context_instance.left = Context_instance.right; \
        Context_instance.PC++;                          \
    }

#define INSTRUCTION_LD_R_N(left)                                                 \
    void instruction_LD_##left##_##n() {                                         \
        Context_instance.left = MemoryManager_ByteRead(Context_instance.PC + 1); \
        Context_instance.PC += 2;                                                \
    }

#define INSTRUCTION_LD_R_HLp(to)                                           \
    void instruction_LD_##to##_HLp() {                                     \
        Context_instance.to = MemoryManager_ByteRead(Context_instance.HL); \
        Context_instance.PC++;                                             \
    }

#define INSTRUCTION_LD_HLp_R(R)                                           \
    void instruction_LD_HLp_##R() {                                       \
        MemoryManager_ByteWrite(Context_instance.HL, Context_instance.R); \
        Context_instance.PC++;                                            \
    }


#define INSTRUCTION_LD_RRp_A(R) \
    void instruction_LD_##R##p_A() {\
        MemoryManager_ByteWrite(Context_instance.R, Context_instance.A);                           \
        Context_instance.PC += 1; \
    }


#define INSTRUCTION_LD_A_RRp(RR)                                          \
    void instruction_LD_A_##RR##p() {                                     \
        Context_instance.A = MemoryManager_ByteRead(Context_instance.RR); \
        Context_instance.PC += 2;                                         \
    }
void instruction_LD_nnp_A() {
    MemoryManager_ByteWrite(MemoryManager_WordRead(Context_instance.PC + 1), Context_instance.A);
    Context_instance.PC+=3;
}

void instruction_LD_A_nnp() {
    Context_instance.A = MemoryManager_ByteRead(MemoryManager_WordRead(Context_instance.PC + 1));
    Context_instance.PC += 3;
}
void instruction_LD_HLp_n() {
    MemoryManager_ByteWrite(Context_instance.HL, MemoryManager_ByteRead(Context_instance.PC+1));
    Context_instance.PC+=2;
}

#define INSTRUCTION_LD_RR_nn(RR)                                               \
    void instruction_LD_##RR##_nn() {                                          \
        Context_instance.RR = MemoryManager_WordRead(Context_instance.PC + 1); \
        Context_instance.PC+=3;                                                 \
    }


#define INSTRUCTION_LD_RR_nnp(RR) \
    void instruction_LD_##RR##_nnp() { \
        Context_instance.RR = MemoryManager_WordRead(MemoryManager_WordRead(Context_instance.PC + 1)); \
        Context_instance.PC+=3;                                                 \
    }


void instruction_LD_nnp_HL() { \
    MemoryManager_WordWrite(MemoryManager_WordRead(Context_instance.PC+1), Context_instance.HL);
    Context_instance.PC+=3;                                                 \
}

void instruction_LD_SP_HL() {
    Context_instance.SP = Context_instance.HL;
    Context_instance.PC++;
}
void instruction_LD_SP_nn() {
    Context_instance.SP = MemoryManager_WordRead(Context_instance.PC + 1);
    Context_instance.PC += 3;
}

#define DEFINE_LD_INSTRUCTION(R) \
    INSTRUCTION_LD_R_R(R, B);    \
    INSTRUCTION_LD_R_R(R, C);    \
    INSTRUCTION_LD_R_R(R, D);    \
    INSTRUCTION_LD_R_R(R, E);    \
    INSTRUCTION_LD_R_R(R, H);    \
    INSTRUCTION_LD_R_R(R, L);    \
    INSTRUCTION_LD_R_R(R, A);    \
    INSTRUCTION_LD_R_N(R);       \
    INSTRUCTION_LD_R_HLp(R);     \
    INSTRUCTION_LD_HLp_R(R);

DEFINE_LD_INSTRUCTION(B)
DEFINE_LD_INSTRUCTION(C)
DEFINE_LD_INSTRUCTION(D)
DEFINE_LD_INSTRUCTION(E)
DEFINE_LD_INSTRUCTION(H)
DEFINE_LD_INSTRUCTION(L)
DEFINE_LD_INSTRUCTION(A)

#define DEFINE_LD_RR_INSTRUCTION(R) \
    INSTRUCTION_LD_A_RRp(R);       \
    INSTRUCTION_LD_RR_nn(R);       \
    INSTRUCTION_LD_RR_nnp(R);      \
    INSTRUCTION_LD_RRp_A(R);

DEFINE_LD_RR_INSTRUCTION(BC)
DEFINE_LD_RR_INSTRUCTION(DE)
INSTRUCTION_LD_RR_nn(HL);
INSTRUCTION_LD_RR_nnp(HL);

void instruction_EX_DE_HL() {
    uint16_t tmp = Context_instance.DE;
    Context_instance.DE = Context_instance.HL;
    Context_instance.HL = tmp;
}
void instruction_EX_SPp_HL() {
    uint16_t tmp = MemoryManager_WordRead(Context_instance.SP);
    MemoryManager_WordWrite(Context_instance.SP, Context_instance.HL);
    Context_instance.HL = tmp;
}
void instruction_EX_AF_AFprime() {
    uint16_t tmp = Context_instance.AF_prime;
    Context_instance.AF_prime = Context_instance.AF;
    Context_instance.AF = tmp;
}
void instruction_EXX() {
    uint16_t tmp = Context_instance.BC;
    Context_instance.BC = Context_instance.BC_prime;
    Context_instance.BC_prime = tmp;

    tmp = Context_instance.DE;
    Context_instance.DE = Context_instance.DE_prime;
    Context_instance.DE_prime = tmp;

    tmp = Context_instance.HL;
    Context_instance.HL = Context_instance.HL_prime;
    Context_instance.HL_prime = tmp;
}


#ifdef DEBUG

void test_instruction_LD() {
    // LD R, R
    // INSTRUCTION_LD_R_R
    Context_instance.A = 0x00;
    Context_instance.B = 0x20;
    uint8_t flag = Context_instance.flags.data;
    check_test_start(instruction_LD_A_B(), Context_instance.A == 0x20);  // set B => A
    check_test(Context_instance.flags.data == flag);

    // LD B, A
    Context_instance.A = 0x25;
    Context_instance.B = 0x00;
    flag = Context_instance.flags.data;
    check_test_start(instruction_LD_B_A(), Context_instance.B == 0x25);  // set A => B
    check_test(Context_instance.flags.data == flag);                     // flag is not changed
    check_PC_test(1);// flag is not changed

    // LD A,n
    // INSTRUCTION_LD_R_N
    Context_instance.A = 0x00;
    MemoryManager_ByteWrite(0x8000 + 1, 0x23);
    check_test_start(instruction_LD_A_n(), Context_instance.A == 0x23);  // set 0x23 => A
    check_test(Context_instance.flags.data == flag);                     // flag is not changed
    check_PC_test(2);


    // LD A, (HL)
    // INSTRUCTION_LD_R_HLp
    Context_instance.HL = 0x8080;
    MemoryManager_ByteWrite(0x8080, 0x28);
    Context_instance.A = 0;
    flag = Context_instance.flags.data;
    check_test_start(instruction_LD_A_HLp(), Context_instance.A == 0x28);
    check_test(Context_instance.flags.data == flag);
    check_PC_test(1);

    // LD (HL), A
    // INSTRUCTION_LD_HLp_R
    Context_instance.HL = 0x8080;
    Context_instance.A = 0x28;
    flag = Context_instance.flags.data;
    check_test_start(instruction_LD_HLp_A(), MemoryManager_ByteRead(0x8080) == 0x28);
    check_test(Context_instance.flags.data == flag);
    check_PC_test(1);

    // INSTRUCTION_LD_RRp_A
    // instruction_LD_nnp_A
    // instruction_LD_A_nnp
    // instruction_LD_SP_HL
    // instruction_LD_SP_nn
    // INSTRUCTION_LD_A_RRp

    // LD BC, 0x1234
    // INSTRUCTION_LD_RR_nn
    MemoryManager_WordWrite(0x8000 + 1, 0x1234);
    Context_instance.A = 0;
    flag = Context_instance.flags.data;
    check_test_start(instruction_LD_BC_nn(), Context_instance.BC == 0x1234);
    check_test(Context_instance.flags.data == flag);
    check_PC_test(3);

    // INSTRUCTION_LD_RR_nnp
    // INSTRUCTION_LD_nnp_RR
}
#endif
