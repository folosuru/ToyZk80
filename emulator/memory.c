#include "emulator.h"


Byte MemoryManager_ByteRead(EmulatorPtr ptr) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff ) {
            return Memory_instance.program[ptr - 0x8000];
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {

        }
    }
}
Word MemoryManager_WordRead(EmulatorPtr ptr) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff ) {
            return *(Word*)(&Memory_instance.program[ptr - 0x8000]);
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {

        }
    }
}
void MemoryManager_ByteWrite(EmulatorPtr, Byte);
void MemoryManager_WordWrite(EmulatorPtr, Word);