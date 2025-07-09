#ifndef TOYZK80EMU_LOGIC_INSTRUCTION_H
#define TOYZK80EMU_LOGIC_INSTRUCTION_H


#define INSTRUCTION_AND_R(R) void instruction_AND_##R();
void instruction_AND_n();
void instruction_AND_HLp();

#define INSTRUCTION_OR_R(R)  void instruction_OR_##R();
void instruction_OR_n();
void instruction_OR_HLp();

#define INSTRUCTION_XOR_R(R) void instruction_XOR_##R();
void instruction_XOR_n();
void instruction_XOR_HLp();

void instruction_RRCA();
void instruction_RLCA();
void instruction_RRA();
void instruction_RLA();

#define DEFINE_LOGIC_INSTRUCTION(R)\
    INSTRUCTION_AND_R(R);     \
    INSTRUCTION_OR_R(R);      \
    INSTRUCTION_XOR_R(R);

DEFINE_LOGIC_INSTRUCTION(B)
DEFINE_LOGIC_INSTRUCTION(C)
DEFINE_LOGIC_INSTRUCTION(D)
DEFINE_LOGIC_INSTRUCTION(E)
DEFINE_LOGIC_INSTRUCTION(H)
DEFINE_LOGIC_INSTRUCTION(L)
DEFINE_LOGIC_INSTRUCTION(A)

#undef INSTRUCTION_AND_R
#undef INSTRUCTION_OR_R
#undef INSTRUCTION_XOR_R
#undef DEFINE_LOGIC_INSTRUCTION

#ifdef DEBUG
void test_instruction_logic();
#endif

#endif  // TOYZK80EMU_LOGIC_INSTRUCTION_H
