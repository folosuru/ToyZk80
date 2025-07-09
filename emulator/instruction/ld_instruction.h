#ifndef TOYZK80EMU_LD_INSTRUCTION_H
#define TOYZK80EMU_LD_INSTRUCTION_H

#define INSTRUCTION_LD_R_R(left, right) void instruction_LD_##left##_##right();
#define INSTRUCTION_LD_R_N(left) void instruction_LD_##left##_##n();
#define INSTRUCTION_LD_R_HLp(to) void instruction_LD_##to##_HLp();
#define INSTRUCTION_LD_HLp_R(R) void instruction_LD_HLp_##R();
#define INSTRUCTION_LD_RRp_A(R) void instruction_LD_##R##p_A();
void instruction_LD_HLp_n();

void instruction_LD_nnp_A();
void instruction_LD_A_nnp();
void instruction_LD_SP_HL();
void instruction_LD_SP_nn();

#define INSTRUCTION_LD_A_RRp(RR) void instruction_LD_A_##RR##p();
#define INSTRUCTION_LD_RR_nn(RR) void instruction_LD_##RR##_nn();
#define INSTRUCTION_LD_RR_nnp(RR) void instruction_LD_##RR##_nnp();
#define INSTRUCTION_LD_nnp_RR(RR) void instruction_LD_nnp_##RR();

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
    INSTRUCTION_LD_HLp_R(R);     \

DEFINE_LD_INSTRUCTION(B)
DEFINE_LD_INSTRUCTION(C)
DEFINE_LD_INSTRUCTION(D)
DEFINE_LD_INSTRUCTION(E)
DEFINE_LD_INSTRUCTION(H)
DEFINE_LD_INSTRUCTION(L)
DEFINE_LD_INSTRUCTION(A)

#define DEFINE_LD_RR_INSTRUCTION(R) \
    INSTRUCTION_LD_A_RRp(R);        \
    INSTRUCTION_LD_RRp_A(R);        \
    INSTRUCTION_LD_RR_nn(R);        \
    INSTRUCTION_LD_RR_nnp(R);\
    INSTRUCTION_LD_nnp_RR(R);\

DEFINE_LD_RR_INSTRUCTION(BC)
DEFINE_LD_RR_INSTRUCTION(DE)
DEFINE_LD_RR_INSTRUCTION(HL)

void instruction_EX_DE_HL();
void instruction_EX_SPp_HL();
void instruction_EX_AF_AFprime();
void instruction_EXX();

#undef INSTRUCTION_LD_R_R
#undef INSTRUCTION_LD_R_N
#undef INSTRUCTION_LD_R_HLp
#undef INSTRUCTION_LD_HLp_R
#undef INSTRUCTION_LD_A_RRp
#undef INSTRUCTION_LD_RRp_A
#undef INSTRUCTION_LD_RR_nn
#undef INSTRUCTION_LD_RR_nnp
#undef INSTRUCTION_LD_nnp_RR
#undef DEFINE_LD_INSTRUCTION
#undef DEFINE_LD_RR_INSTRUCTION

#ifdef DEBUG
void test_instruction_LD();
#endif

#endif  // TOYZK80EMU_LD_INSTRUCTION_H
