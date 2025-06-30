#ifndef TOYTK80_STACK_INSTRUCTION_H
#define TOYTK80_STACK_INSTRUCTION_H

void instruction_PUSH_BC();
void instruction_PUSH_DE();
void instruction_PUSH_HL();
void instruction_PUSH_AF();
void instruction_POP_BC();
void instruction_POP_DE();
void instruction_POP_HL();
void instruction_POP_AF();

#ifdef DEBUG
void test_instruction_stack();
#endif
#endif  // TOYTK80_STACK_INSTRUCTION_H
