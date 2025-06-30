#include "curses.h"
#include "emulator.h"
#include <unistd.h>

void  init_emulator() {
    Context_instance.PC = 0x8000;
    return;
}

_Noreturn void mainloop() {
    InstructionPtrTable table = InstructionTable();
    while (true) {
        usleep(1000);
        (*(*table)[MemoryManager_ByteRead(Context_instance.PC)])();
        Context_instance.R++;
    }
}

Byte MemoryManager_ByteRead(EmulatorPtr ptr) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff ) {
            return Memory_instance.program[ptr - 0x8000];
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            return Memory_instance.Display[ptr - 0x83f8];
        }
    }
    return AccessViolation(ptr);
}
Word MemoryManager_WordRead(EmulatorPtr ptr) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff ) {
            return *(Word*)(&Memory_instance.program[ptr - 0x8000]);
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            return *(Word*)(&Memory_instance.Display[ptr - 0x83f8]);
        }
    }
    return AccessViolation(ptr);
}
void MemoryManager_ByteWrite(EmulatorPtr ptr, Byte data) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff ) {
            *(Byte*)(&Memory_instance.program[ptr - 0x8000]) = data;
            return;
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            *(Byte*)(&Memory_instance.Display[ptr - 0x83f8]) = data;
            UpdateDisplay(ptr - 0x83f8);
            refresh();
            return;
        }
    }
    AccessViolation(ptr);
}
void MemoryManager_WordWrite(EmulatorPtr ptr, Word data) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff ) {
            *(Word*)(&Memory_instance.program[ptr - 0x8000]) = data;
            return;
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            *(Word*)(&Memory_instance.Display[ptr - 0x83f8]) = data;
            UpdateDisplay(ptr - 0x83f8);
            UpdateDisplay(ptr - 0x83f8 + 1);
            refresh();
            return;
        }
    }
    AccessViolation(ptr);
}



