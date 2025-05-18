#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
struct ASM_Instruction {
    const char *instruction; // "LD"
    const char *operand;  //  "IY,nn"
    unsigned char OpCode[4]; // 0xFD 0x21
    char OpCodeCount;  // 2
};

/**
 * A, F, B, C, D, E, H, L, I, R : 8bit register
 * AF, BC, DE, HL, IX, IY, SP : 16bit register
 *
 * n  : 8bit number
 * nn : 16bit number
 * e  : label or address
 *
 * delimiter: sore igai
 */
extern  struct ASM_Instruction ASM_instructions[];
extern int ASM_Instructions_count;

struct Opcode_data {
    enum {
        Opcode_data_1byte,
        Opcode_data_2byte,
        Opcode_data_label,
    } type;
    union {
        int byte;
        struct {
            const char* name;
            int length;
        } label;
    };
};

typedef struct {
    const struct ASM_Instruction* instruction;
    struct Opcode_data data[6];
    int data_len;
} ASM_Opcode;
#endif //INSTRUCTIONS_H
