#pragma once
#ifndef TOYTK80_EMULATOR_H
#define TOYTK80_EMULATOR_H

#include <stdint.h>

typedef uint8_t Byte;
typedef uint16_t Word;
typedef size_t EmulatorPtr;

struct Context {
    // Main Register Set
    union {
        uint16_t AF;
        struct { uint8_t F, A; };
    };
    union {
        uint16_t BC;
        struct { uint8_t C, B; };
    };
    union {
        uint16_t DE;
        struct { uint8_t E, D; };
    };
    union {
        uint16_t HL;
        struct { uint8_t L, H; };
    };

    // Alternate (Shadow) Register Set
    union {
        uint16_t AF_prime;
        struct { uint8_t F_prime, A_prime; };
    };
    union {
        uint16_t BC_prime;
        struct { uint8_t C_prime, B_prime; };
    };
    union {
        uint16_t DE_prime;
        struct { uint8_t E_prime, D_prime; };
    };
    union {
        uint16_t HL_prime;
        struct { uint8_t L_prime, H_prime; };
    };

    // Index Registers
    uint16_t IX;
    uint16_t IY;

    // Stack Pointer & Program Counter
    uint16_t SP;
    uint16_t PC;

    // Interrupt Vector & Memory Refresh
    uint8_t I;
    uint8_t R;

    // flags
    union {
        struct {
            uint8_t SF : 1;
            uint8_t ZF : 1;
            uint8_t HC : 1;
            uint8_t PV : 1;  // P/V (overflow)
            uint8_t NF : 1;
            uint8_t CF : 1;  // carry
        };
        uint8_t data;
    }flags;
};

struct Memory {
    Byte program[512];
    Byte Display[8];
};

typedef void (*InstructionPtr)();
extern struct Context Context_instance;
extern struct Memory Memory_instance;

void  init_emulator();

Byte MemoryManager_ByteRead(EmulatorPtr);
Word MemoryManager_WordRead(EmulatorPtr);
void MemoryManager_ByteWrite(EmulatorPtr, Byte);
void MemoryManager_WordWrite(EmulatorPtr, Word);

void UpdateDisplay(int digit);

void Execute();
int AccessViolation(EmulatorPtr);

#ifdef DEBUG
void test_instruction();
#endif

#endif //TOYTK80_EMULATOR_H
