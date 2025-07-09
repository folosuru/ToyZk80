#ifndef TOYZK80EMU_JUMP_INSTRUCTION_H
#define TOYZK80EMU_JUMP_INSTRUCTION_H

void instruction_JP_nn();
void instruction_JP_NZ_nn();
void instruction_JP_Z_nn();
void instruction_JP_NC_nn();
void instruction_JP_C_nn();
void instruction_JP_PO_nn();
void instruction_JP_PE_nn();
void instruction_JP_P_nn();
void instruction_JP_M_nn();
void instruction_JR_e();
void instruction_JR_C_e();
void instruction_JR_NC_e();
void instruction_JR_Z_e();
void instruction_JR_NZ_e();
void instruction_JP_HLp();
void instruction_DJNZ_e();


#ifdef DEBUG
void test_instruction_jump();
#endif

#endif  // TOYZK80EMU_JUMP_INSTRUCTION_H
