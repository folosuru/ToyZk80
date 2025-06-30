#include <stdlib.h>
#include <string.h>

#include "curses.h"
#include "emulator.h"

#include "instruction/instruction_test_util.h"
#include "instruction/logic_instruction.h"
#include "instruction/math_instruction.h"
#include "instruction/ld_instruction.h"
#include "instruction/condition_instruction.h"
#include "instruction/unimplement_instruction.h"
#include "instruction/jump_instruction.h"
#include "instruction/stack_instruction.h"

#ifdef DEBUG
const char* last_test_instruction;
#endif

/**
 * Implement of Z80 instruction.
 *
 **/

void instruction_NOP() {
    Context_instance.PC++;
}

void instruction_HALT() {
    mvaddstr(24, 5, "program halted.");
    getch();
    exit(0);
    Context_instance.PC++;
}


InstructionPtrTable InstructionTable() {
    // instruction list

    InstructionPtr table[256] = {
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
    };

    InstructionPtrTable table_ptr = malloc(sizeof(table));
    memcpy(table_ptr, &table, sizeof(table));
    return table_ptr;
}

#ifdef DEBUG
void test_instruction() {
    test_instruction_LD();
    test_instruction_math();
    test_instruction_logic();
    test_instruction_jump();
    test_instruction_stack();
}

#endif
