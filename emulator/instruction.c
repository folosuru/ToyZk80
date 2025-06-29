#include <stdlib.h>

#include "curses.h"
#include "emulator.h"
/**
 * Implement of Z80 instruction.
 *
 **/

/**
 * LD instructions
 *
 */

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

#define INSTRUCTION_LD_A_RRp(RR)                                          \
    void instruction_LD_A_##RR##p() {                                     \
        Context_instance.A = MemoryManager_ByteRead(Context_instance.RR); \
        Context_instance.PC += 2;                                         \
    }

#define INSTRUCTION_LD_RR_nn(RR)                                               \
    void instruction_LD_##RR##_nn() {                                          \
        Context_instance.RR = MemoryManager_WordRead(Context_instance.PC + 1); \
        Context_instance.PC++;                                                 \
    }

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

// x64 seto = Z80 PV
// Z80's HC isn't exist in x64 ?
#define INSTRUCTION_ADD_A_R(add_register)                                                                 \
    void instruction_ADD_A_##add_register() {                                                             \
        __asm__(                                                                                          \
            "add %0, %5\n\t"                                                                              \
            "sets %1\n\t"                                                                                 \
            "setz %2\n\t"                                                                                 \
            "seto %3\n\t"                                                                                 \
            "setc %4\n\t"                                                                                 \
            : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
            : "r"(Context_instance.add_register));                                                        \
        Context_instance.flags.NF = 1;                                                                    \
    }

#define INSTRUCTION_ADC_A_R(add_register)                                                                 \
    void instruction_ADC_A_##add_register() {                                                             \
        __asm__(                                                                                          \
            "adc %0, %5\n\t"                                                                              \
            "sets %1\n\t"                                                                                 \
            "setz %2\n\t"                                                                                 \
            "seto %3\n\t"                                                                                 \
            "setc %4\n\t"                                                                                 \
            : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
            : "r"(Context_instance.add_register));                                                        \
        Context_instance.flags.NF = 1;                                                                    \
    }

#define INSTRUCTION_SUB_A_R(sub_register)                                                                 \
    void instruction_SUB_##sub_register() {                                                               \
        __asm__(                                                                                          \
            "sub %0, %5\n\t"                                                                              \
            "sets %1\n\t"                                                                                 \
            "setz %2\n\t"                                                                                 \
            "seto %3\n\t"                                                                                 \
            "setc %4\n\t"                                                                                 \
            : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
            : "r"(Context_instance.sub_register));                                                        \
        Context_instance.flags.NF = 0;                                                                    \
    }

#define INSTRUCTION_SBC_A_R(sub_register)                                                                 \
    void instruction_SBC_##sub_register() {                                                               \
        __asm__(                                                                                          \
            "sbb %0, %5\n\t"                                                                              \
            "sets %1\n\t"                                                                                 \
            "setz %2\n\t"                                                                                 \
            "seto %3\n\t"                                                                                 \
            "setc %4\n\t"                                                                                 \
            : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)                            \
            : "r"(Context_instance.sub_register));                                                        \
        Context_instance.flags.NF = 0;                                                                    \
    }

#define INSTRUCTION_AND_R(R)                                                                              \
    void instruction_AND_##R() {                                                                          \
        uint16_t popcnt_result;                                                                           \
        __asm__(                                                                                          \
            "and %0, %4\n\t"                                                                              \
            "sets %1\n\t"                                                                                 \
            "setz %2\n\t"                                                                                 \
            "mov ah, 0\n\r"                                                                               \
            "popcnt %3, ax\n\t" /* for before Nehalem gen user: sorry.*/                                  \
            : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF), \
              "=r"(popcnt_result)                                                                         \
            : "r"(Context_instance.R));                                                                   \
        Context_instance.flags.NF = 0;                                                                    \
        Context_instance.flags.CF = 0;                                                                    \
        Context_instance.flags.PV = popcnt_result;                                                        \
    }

#define INSTRUCTION_UNIMPLEMENT_TEMPLATE(name) \
    void instruction_##name() {                \
        Context_instance.PC++;                 \
    }

void instruction_NOP() {
    Context_instance.PC++;
}

#define INSTRUCTION_R_ALL(R)  \
    INSTRUCTION_LD_R_R(R, B); \
    INSTRUCTION_LD_R_R(R, C); \
    INSTRUCTION_LD_R_R(R, D); \
    INSTRUCTION_LD_R_R(R, E); \
    INSTRUCTION_LD_R_R(R, H); \
    INSTRUCTION_LD_R_R(R, L); \
    INSTRUCTION_LD_R_R(R, A); \
    INSTRUCTION_LD_R_N(R);    \
    INSTRUCTION_LD_R_HLp(R);  \
    INSTRUCTION_LD_HLp_R(R);  \
    INSTRUCTION_INC_R(R);     \
    INSTRUCTION_DEC_R(R);     \
    INSTRUCTION_ADD_A_R(R);   \
    INSTRUCTION_ADC_A_R(R);   \
    INSTRUCTION_SUB_A_R(R);   \
    INSTRUCTION_SBC_A_R(R);   \
    INSTRUCTION_AND_R(R)

INSTRUCTION_R_ALL(B);
INSTRUCTION_R_ALL(C);
INSTRUCTION_R_ALL(D);
INSTRUCTION_R_ALL(E);
INSTRUCTION_R_ALL(H);
INSTRUCTION_R_ALL(L);
INSTRUCTION_R_ALL(A);

#define INSTRUCTION_RR_ALL(R) INSTRUCTION_LD_A_RRp(R) INSTRUCTION_INC_R(R) INSTRUCTION_DEC_R(R) INSTRUCTION_LD_RR_nn(R)

INSTRUCTION_RR_ALL(BC);
INSTRUCTION_RR_ALL(DE);

void instruction_HLp_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    MemoryManager_ByteWrite(Context_instance.HL, operand);
    Context_instance.PC += 2;
}

void instruction_ADD_A_n() {
    Byte operand = MemoryManager_ByteRead(Context_instance.PC + 1);
    __asm__(
        "add %0, %3\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        "setc %4\n\t"
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)
        : "r"(operand));
    Context_instance.flags.NF = 1;
    Context_instance.PC += 2;
}

void instruction_ADD_A_HLp() {
    __asm__(
        "add %0, %5\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        "setc %4\n\t"
        : "+r"(Context_instance.A), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)
        : "r"(MemoryManager_ByteRead(Context_instance.HL)));
    Context_instance.flags.NF = 1;
}

void instruction_LD_BCp_A() {
    MemoryManager_ByteWrite(Context_instance.BC, Context_instance.A);
    Context_instance.PC++;
}

void instruction_LD_() {
    MemoryManager_ByteWrite(Context_instance.BC, Context_instance.A);
    Context_instance.PC++;
}

void instruction_DEC_HLp() {
    Byte result = MemoryManager_ByteRead(Context_instance.HL);
    __asm__(
        "dec %0\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        "setc %4\n\t"
        : "+r"(result), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)
        :);
    MemoryManager_ByteWrite(Context_instance.HL, result);
    Context_instance.flags.NF = 0;
    Context_instance.PC++;
}

void instruction_INC_HLp() {
    Byte result = MemoryManager_ByteRead(Context_instance.HL);
    __asm__(
        "inc %0\n\t"
        "sets %1\n\t"
        "setz %2\n\t"
        "seto %3\n\t"
        "setc %4\n\t"
        : "+r"(result), "=r"(Context_instance.flags.SF), "=r"(Context_instance.flags.ZF),
          "=r"(Context_instance.flags.PV), "=r"(Context_instance.flags.CF)
        :);
    MemoryManager_ByteWrite(Context_instance.HL, result);
    Context_instance.flags.NF = 1;
    Context_instance.PC++;
}

void instruction_HALT() {
    getch();
    Context_instance.PC++;
}

void InstructionTable() {
    // instruction list

    InstructionPtr table[256] = {
        /*
instruction_NOP,           instruction_LD_BC_nn,   instruction_LD_BCp_A,  instruction_INC_BC,
instruction_INC_B,         instruction_DEC_B,      instruction_LD_B_n,    instruction_RLCA,
instruction_EX_AF_AFprime, instruction_ADD_HL_BC,  instruction_LD_A_BCp,  instruction_DEC_BC,
instruction_INC_C,         instruction_DEC_C,      instruction_LD_C_n,    instruction_RRCA,
instruction_DJNZ_e,        instruction_LD_DE_nn,   instruction_LD_DEp_A,  instruction_INC_DE,
instruction_INC_D,         instruction_DEC_D,      instruction_LD_D_n,    instruction_RLA,
instruction_JR_e,          instruction_ADD_HL_DE,  instruction_LD_A_DEp,  instruction_DEC_DE,
instruction_INC_E,         instruction_DEC_E,      instruction_LD_E_n,    instruction_RRA,
instruction_JR_NZ_e,       instruction_LD_HL_nn,   instruction_LD_nnp_HL, instruction_INC_HL,
instruction_INC_H,         instruction_DEC_H,      instruction_LD_H_n,    instruction_DAA,
instruction_JR_Z_e,        instruction_ADD_HL_HL,  instruction_LD_HL_nnp, instruction_DEC_HL,
instruction_INC_L,         instruction_DEC_L,      instruction_LD_L_n,    instruction_CPL,
instruction_JR_NC_e,       instruction_LD_SP_nn,   instruction_LD_nnp_A,  instruction_INC_SP,
instruction_INC_HLp,       instruction_DEC_HLp,    instruction_LD_HLp_n,  instruction_SCF,
instruction_JR_C_e,        instruction_ADD_HL_SP,  instruction_LD_A_nnp,  instruction_DEC_SP,
instruction_INC_A,         instruction_DEC_A,      instruction_LD_A_n,    instruction_CCF,
instruction_LD_B_B,        instruction_LD_B_C,     instruction_LD_B_D,    instruction_LD_B_E,
instruction_LD_B_H,        instruction_LD_B_L,     instruction_LD_B_HLp,  instruction_LD_B_A,
instruction_LD_C_B,        instruction_LD_C_C,     instruction_LD_C_D,    instruction_LD_C_E,
instruction_LD_C_H,        instruction_LD_C_L,     instruction_LD_C_HLp,  instruction_LD_C_A,
instruction_LD_D_B,        instruction_LD_D_C,     instruction_LD_D_D,    instruction_LD_D_E,
instruction_LD_D_H,        instruction_LD_D_L,     instruction_LD_D_HLp,  instruction_LD_D_A,
instruction_LD_E_B,        instruction_LD_E_C,     instruction_LD_E_D,    instruction_LD_E_E,
instruction_LD_E_H,        instruction_LD_E_L,     instruction_LD_E_HLp,  instruction_LD_E_A,
instruction_LD_H_B,        instruction_LD_H_C,     instruction_LD_H_D,    instruction_LD_H_E,
instruction_LD_H_H,        instruction_LD_H_L,     instruction_LD_H_HLp,  instruction_LD_H_A,
instruction_LD_L_B,        instruction_LD_L_C,     instruction_LD_L_D,    instruction_LD_L_E,
instruction_LD_L_H,        instruction_LD_L_L,     instruction_LD_L_HLp,  instruction_LD_L_A,
instruction_LD_HLp_B,      instruction_LD_HLp_C,   instruction_LD_HLp_D,  instruction_LD_HLp_E,
instruction_LD_HLp_H,      instruction_LD_HLp_L,   instruction_HALT,      instruction_LD_HLp_A,
instruction_LD_A_B,        instruction_LD_A_C,     instruction_LD_A_D,    instruction_LD_A_E,
instruction_LD_A_H,        instruction_LD_A_L,     instruction_LD_A_HLp,  instruction_LD_A_A,
instruction_ADD_A_B,       instruction_ADD_A_C,    instruction_ADD_A_D,   instruction_ADD_A_E,
instruction_ADD_A_H,       instruction_ADD_A_L,    instruction_ADD_A_HLp, instruction_ADD_A_A,
instruction_ADC_A_B,       instruction_ADC_A_C,    instruction_ADC_A_D,   instruction_ADC_A_E,
instruction_ADC_A_H,       instruction_ADC_A_L,    instruction_ADC_A_HLp, instruction_ADC_A_A,
instruction_SUB_B,         instruction_SUB_C,      instruction_SUB_D,     instruction_SUB_E,
instruction_SUB_H,         instruction_SUB_L,      instruction_SUB_HLp,   instruction_SUB_A,
instruction_SBC_B,         instruction_SBC_C,      instruction_SBC_D,     instruction_SBC_E,
instruction_SBC_H,         instruction_SBC_L,      instruction_SBC_HLp,   instruction_SBC_A,
instruction_AND_B,         instruction_AND_C,      instruction_AND_D,     instruction_AND_E,
instruction_AND_H,         instruction_AND_L,      instruction_AND_HLp,   instruction_AND_A,
instruction_XOR_B,         instruction_XOR_C,      instruction_XOR_D,     instruction_XOR_E,
instruction_XOR_H,         instruction_XOR_L,      instruction_XOR_HLp,   instruction_XOR_A,
instruction_OR_B,          instruction_OR_C,       instruction_OR_D,      instruction_OR_E,
instruction_OR_H,          instruction_OR_L,       instruction_OR_HLp,    instruction_OR_A,
instruction_CP_B,          instruction_CP_C,       instruction_CP_D,      instruction_CP_E,
instruction_CP_H,          instruction_CP_L,       instruction_CP_HLp,    instruction_CP_A,
instruction_RET_NZ,        instruction_POP_BC,     instruction_JP_NZ_nn,  instruction_JP_nn,
instruction_CALL_NZ_nn,    instruction_PUSH_BC,    instruction_ADD_A_n,   instruction_RST_00H,
instruction_RET_Z,         instruction_RET,        instruction_JP_Z_nn,   instruction_Section_CB,
instruction_CALL_Z_nn,     instruction_CALL_nn,    instruction_ADC_A_n,   instruction_RST_08H,
instruction_RET_NC,        instruction_POP_DE,     instruction_JP_NC_nn,  instruction_OUT_np_A,
instruction_CALL_NC_nn,    instruction_PUSH_DE,    instruction_SUB_n,     instruction_RST_10H,
instruction_RET_C,         instruction_EXX,        instruction_JP_C_nn,   instruction_IN_A_np,
instruction_CALL_C_nn,     instruction_Section_DD, instruction_SBC_A_n,   instruction_RST_18H,
instruction_RET_PO,        instruction_POP_HL,     instruction_JP_PO_nn,  instruction_EX_SPp_HL,
instruction_CALL_PO_nn,    instruction_PUSH_HL,    instruction_AND_n,     instruction_RST_20H,
instruction_RET_PE,        instruction_JP_HLp,     instruction_JP_PE_nn,  instruction_EX_DE_HL,
instruction_CALL_PE_nn,    instruction_Section_ED, instruction_XOR_n,     instruction_RST_28H,
instruction_RET_P,         instruction_POP_AF,     instruction_JP_P_nn,   instruction_DI,
instruction_CALL_P_nn,     instruction_PUSH_AF,    instruction_OR_n,      instruction_RST_30H,
instruction_RET_M,         instruction_LD_SP_HL,   instruction_JP_M_nn,   instruction_EI,
instruction_CALL_M_nn,     instruction_Section_FD, instruction_CP_n,      instruction_RST_38H,
*/
    };

    malloc(sizeof(table));
}

#ifdef DEBUG
const char* last_test_instruction;

#define check_test_start(instruction, debug_stm)                                                \
    do {                                                                                        \
        instruction;                                                                            \
        last_test_instruction = #instruction;                                                   \
        if (!(debug_stm)) {                                                                     \
            fprintf(stderr, "test: " #instruction " is fail.\n    " #debug_stm " is false.\n"); \
        }                                                                                       \
    } while (0)

#define check_test(debug_stm)                                                                            \
    do {                                                                                                 \
        if (!(debug_stm)) {                                                                              \
            fprintf(stderr, "test: %s is fail.\n    " #debug_stm " is false.\n", last_test_instruction); \
        }                                                                                                \
    } while (0)

void test_instruction() {
    // LD test

    // LD A. B
    Context_instance.A = 0x00;
    Context_instance.B = 0x20;
    uint8_t flag = Context_instance.flags.data;
    check_test_start(instruction_LD_A_B(), Context_instance.A == 0x20);  // set B => A
    check_test(Context_instance.flags.data == flag);                     // flag is not changed

    // LD A,n
    Context_instance.A = 0x00;
    Context_instance.PC = 0x8000;
    MemoryManager_ByteWrite(Context_instance.PC + 1, 0x23);
    check_test_start(instruction_LD_A_n(), Context_instance.A == 0x23);  // set 0x23 => A
    check_test(Context_instance.flags.data == flag);                     // flag is not changed

    // LD B, A
    Context_instance.B = 0x00;
    Context_instance.A = 0x25;
    flag = Context_instance.flags.data;
    check_test_start(instruction_LD_B_A(), Context_instance.B == 0x25);  // set A => B
    check_test(Context_instance.flags.data == flag);                     // flag is not changed

    // LD (HL), A
    Context_instance.HL = 0x8080;
    Context_instance.A = 0x28;
    flag = Context_instance.flags.data;
    check_test_start(instruction_LD_HLp_A(), MemoryManager_ByteRead(0x8080) == 0x28);
    check_test(Context_instance.flags.data == flag);

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

    Context_instance.A = 0xAA;  // 1010 1010
    Context_instance.B = 0x33;  // 0011 0011
    // 0x22 = 0010 0010
    check_test_start(instruction_AND_B(), Context_instance.A == 0x22);
    printf("%x", Context_instance.A );
    check_test(Context_instance.flags.SF == 0);
    check_test(Context_instance.flags.ZF == 0);
    check_test(Context_instance.flags.HC == 1);
    check_test(Context_instance.flags.PV == 0);
    check_test(Context_instance.flags.NF == 0);
    check_test(Context_instance.flags.CF == 0);
}

#endif
