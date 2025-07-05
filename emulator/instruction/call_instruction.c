#include "call_instruction.h"
#include "../emulator.h"


void instruction_RET() {
    Context_instance.PC = 0;
    Context_instance.PC |= (MemoryManager_ByteRead(Context_instance.SP+1) << 8);
    Context_instance.PC |= MemoryManager_ByteRead(Context_instance.SP);
    Context_instance.SP += 2;
}
void instruction_CALL_nn() {
    Context_instance.PC += 3;
    MemoryManager_ByteWrite(Context_instance.SP-1, Context_instance.PC >> 8);
    MemoryManager_ByteWrite(Context_instance.SP-2, Context_instance.PC);
    Context_instance.SP -= 2;
    Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1-3);
}