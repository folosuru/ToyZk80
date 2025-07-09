#ifndef TOYZK80EMU_MATH_INSTRUCTION_H
#define TOYZK80EMU_MATH_INSTRUCTION_H


#define INSTRUCTION_ADD_A_R(add_register) void instruction_ADD_A_##add_register();
void instruction_ADD_A_HLp();
void instruction_ADD_A_n();

#define INSTRUCTION_ADC_A_R(add_register) void instruction_ADC_A_##add_register();
void instruction_ADC_A_n();
void instruction_ADC_A_HLp();

#define INSTRUCTION_SUB_A_R(sub_register) void instruction_SUB_##sub_register();
void instruction_SUB_HLp();
void instruction_SUB_n();


#define INSTRUCTION_SBC_A_R(sub_register) void instruction_SBC_##sub_register();
void instruction_SBC_A_n();
void instruction_SBC_HLp();

#define INSTRUCTION_INC_R(RR) void instruction_INC_##RR();
void instruction_INC_HLp();

#define INSTRUCTION_DEC_R(R) void instruction_DEC_##R();
void instruction_DEC_HLp();

#define INSTRUCTION_ADD_HL_RR(R) void instruction_ADD_HL_##R();



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

#define DEFINE_MATH_INSTRUCTION_RR(R) INSTRUCTION_INC_R(R) INSTRUCTION_DEC_R(R) INSTRUCTION_ADD_HL_RR(R)

DEFINE_MATH_INSTRUCTION_RR(BC)
DEFINE_MATH_INSTRUCTION_RR(DE)
DEFINE_MATH_INSTRUCTION_RR(HL)
DEFINE_MATH_INSTRUCTION_RR(SP)


#undef INSTRUCTION_INC_R
#undef INSTRUCTION_DEC_R
#undef INSTRUCTION_ADD_A_R
#undef INSTRUCTION_ADC_A_R
#undef INSTRUCTION_SUB_A_R
#undef INSTRUCTION_SBC_A_R
#undef INSTRUCTION_ADD_HL_RR
#undef DEFINE_MATH_INSTRUCTION
#undef DEFINE_MATH_INSTRUCTION_RR

#ifdef DEBUG
void test_instruction_math();
#endif
#endif  // TOYZK80EMU_MATH_INSTRUCTION_H
