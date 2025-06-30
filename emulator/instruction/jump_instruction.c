#include "jump_instruction.h"
#include "instruction_test_util.h"
#include "../emulator.h"

void instruction_JP_nn() {
    Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
}

void instruction_JP_NZ_nn() {
    if (!Context_instance.flags.ZF) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_Z_nn() {
    if (Context_instance.flags.ZF) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_NC_nn() {
    if (!Context_instance.flags.CF) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_C_nn() {
    if (Context_instance.flags.CF) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_PO_nn() {
    if (!Context_instance.flags.PV) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_PE_nn() {
    if (Context_instance.flags.PV) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_P_nn() {
    if (!Context_instance.flags.SF) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JP_M_nn() {
    if (Context_instance.flags.SF) {
        Context_instance.PC = MemoryManager_WordRead(Context_instance.PC + 1);
    } else {
        Context_instance.PC += 3;
    }
}
void instruction_JR_e() {
    Context_instance.PC += (int8_t)MemoryManager_ByteRead(Context_instance.PC+1);
}
void instruction_JR_C_e() {
    if (Context_instance.flags.CF) {
        Context_instance.PC += (int8_t)MemoryManager_ByteRead(Context_instance.PC+1);
    } else {
        Context_instance.PC += 2;
    }
}
void instruction_JR_NC_e() {
    if (!Context_instance.flags.CF) {
        Context_instance.PC += (int8_t)MemoryManager_ByteRead(Context_instance.PC+1);
    } else {
        Context_instance.PC += 2;
    }
}
void instruction_JR_Z_e() {
    if (Context_instance.flags.ZF) {
        Context_instance.PC += (int8_t)MemoryManager_ByteRead(Context_instance.PC+1);
    } else {
        Context_instance.PC += 2;
    }
}
void instruction_JR_NZ_e() {
    if (!Context_instance.flags.ZF) {
        Context_instance.PC += (int8_t)MemoryManager_ByteRead(Context_instance.PC+1);
    } else {
        Context_instance.PC += 2;
    }
}

void instruction_JP_HLp() {
    Context_instance.PC = MemoryManager_WordRead(Context_instance.HL);
}

void instruction_DJNZ_e();

extern void instruction_ADD_A_B();

void test_instruction_jump() {
    MemoryManager_WordWrite(0x8000+1, 0x8080);
    check_test_start(instruction_JP_nn(), Context_instance.PC == 0x8080);

    Context_instance.A = 0;
    Context_instance.B = 0;
    instruction_ADD_A_B();
    MemoryManager_WordWrite(0x8000+1, 0x8080);
    check_test_start(instruction_JP_Z_nn(), Context_instance.PC == 0x8080);

    Context_instance.A = 0;
    Context_instance.B = 1;
    instruction_ADD_A_B();
    MemoryManager_WordWrite(0x8000+1, 0x8080);
    check_test_start(instruction_JP_Z_nn(), 1==1);
    check_PC_test(3);

    MemoryManager_ByteWrite(0x8000+1, 0xFF);
    check_test_start(instruction_JR_e(), Context_instance.PC == 0x8000-1);
}
