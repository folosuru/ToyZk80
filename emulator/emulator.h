#pragma once
#ifndef TOYTK80_EMULATOR_H
#define TOYTK80_EMULATOR_H

#include <stdint.h>
#include <curses.h>

typedef uint8_t Byte;
typedef uint16_t Word;
typedef size_t EmulatorPtr;

struct Context {
    // Main Register Set
    union {
        uint16_t AF;
        struct {
            union {
                struct {
                    uint8_t SF : 1;
                    uint8_t ZF : 1;
                    uint8_t HC : 1;
                    uint8_t PV : 1;  // P/V (overflow)
                    uint8_t NF : 1;
                    uint8_t CF : 1;  // carry
                };
                uint8_t F;
            };
            uint8_t A;
        };
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

};

struct Memory {
    Byte program[512];
    Byte Display[8];
};

typedef void (*InstructionPtr)();
typedef const InstructionPtr (*InstructionPtrTable)[256];

InstructionPtrTable InstructionTable();

extern struct Context Context_instance;
extern struct Memory Memory_instance;

_Noreturn void mainloop();
void run_loop(const InstructionPtrTable table);

void  init_emulator();

Byte MemoryManager_ByteRead(EmulatorPtr);
Word MemoryManager_WordRead(EmulatorPtr);
void MemoryManager_ByteWrite(EmulatorPtr, Byte);
void MemoryManager_WordWrite(EmulatorPtr, Word);

void UpdateDisplay(int digit);

int AccessViolation(EmulatorPtr);

enum running_status {
    status_run,
    status_pause,
    status_violation,
    status_halt
};;

extern enum running_status running_operation_status;
extern WINDOW * LED_window;
extern WINDOW * information_window;
extern WINDOW * status_window;
extern WINDOW * control_window;
void init_display();
void print_register_window();
void print_control_window();

#ifdef DEBUG
void test_instruction();
#endif

#endif //TOYTK80_EMULATOR_H
