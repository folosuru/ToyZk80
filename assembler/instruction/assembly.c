#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"

#define ARRAY_SIZEOF(arr) sizeof(arr) / sizeof(arr[0])

int getOpcodeSize(const struct ASM_Instruction* instruction) {
    if (instruction == NULL) {
        return 1;
    }

    int result = (int)instruction->OpCodeCount;

    for (int i = 0; i < sizeof(instruction->data) / sizeof(instruction->data[0]); i++) {
        switch (instruction->data[i]) {
            case data_none:
                return result;
            case data_n:
            case data_e:
            case data_d:
                result += 1;
                break;

            case data_nn:
                result += 2;
                break;
        }
    }
    return result;
}


bool read_operand_code(const unsigned char* data_start, int* data_pos, int data_len, ASM_Opcode* result) {
    const struct ASM_Instruction* instruction = result->instruction;

    int start_data_pos = *data_pos;
    for (int i = 0; i < ARRAY_SIZEOF(instruction->data); i++) {
        if (instruction->data[i] == data_none) {
            break;
        }
        if (data_len <= *data_pos) {
            *data_pos = start_data_pos;
            return false;
        }

        result->data_len++;
        if (instruction->data[i] == data_n) {
            result->data[i].type = Opcode_data_1byte;
            result->data[i].data = *(data_start + (*data_pos));
            (*data_pos)++;
        } else if (instruction->data[i] == data_nn) {
            if (data_len <= (*data_pos) + 1) {
                *data_pos = start_data_pos;
                return false;
            }

            result->data[i].type = Opcode_data_2byte;
            result->data[i].data = *(data_start + (*data_pos)) | (*(data_start + (*data_pos + 1)) << 8);
            (*data_pos) += 2;

        } else if (instruction->data[i] == data_e) {
            result->data[i].type = Opcode_data_offset;
            result->data[i].s1bit_data = *(signed char*)(data_start + (*data_pos));
            (*data_pos)++;
        }
    }
    return true;
}

bool find_instruction_by_code(const unsigned char* data_start, int* data_pos, int data_len, ASM_Opcode* result) {
    int pos_start = *data_pos;
    for (int i = 0; i < ASM_Instructions_count; i++) {
        struct ASM_Instruction* instruction = &ASM_instructions[i];

        if (instruction->OpCode[0] != *(data_start + (*data_pos))) {
            continue;
        }
        *data_pos = pos_start;

        (*data_pos)++;
        bool do_continue = false;
        for (int j = 1; j < instruction->OpCodeCount; j++) {
            if ((*data_pos) == data_len || instruction->OpCode[j] != *(data_start + (*data_pos))) {
                do_continue = true;
                break;
            }
            (*data_pos)++;
        }
        if (do_continue) {
            continue;
        }
        result->instruction = &ASM_instructions[i];
        if (read_operand_code(data_start, data_pos, data_len, result)) {
            return true;
        }
    }
    *data_pos = pos_start;
    return false;
}


void Opcode_to_ASM_str(ASM_Opcode* opcode, char* buffer, int buffer_len) {
    int len = strlen(opcode->instruction->instruction);
    memcpy(buffer, opcode->instruction->instruction, len);

    char* current_buf = buffer + len;
    *current_buf = ' ';
    current_buf++;
    const char* operand_pos = opcode->instruction->operand;
    int operand_cnt = 0;
    while ((current_buf - buffer < buffer_len-1) && (*operand_pos != '\0')) {
        if (*operand_pos == 'n') {
            if (*(operand_pos+1) == 'n') {
                operand_pos++;
                int wrote = snprintf(current_buf, buffer_len - (current_buf - buffer),
                                     "%04x", opcode->data[operand_cnt].data );
                current_buf += wrote;
            } else {
                int wrote = snprintf(current_buf, buffer_len - (current_buf - buffer),
                                     "%02x", opcode->data[operand_cnt].data);
                current_buf += wrote;
            }
            operand_pos++;
            operand_cnt++;
            continue;
        } else  if (*operand_pos == 'e') {
            int wrote = snprintf(current_buf, buffer_len - (current_buf - buffer),
                                 "%02x", opcode->data[operand_cnt].data & 0xFF);
            current_buf += wrote;
            operand_pos++;
            operand_cnt++;
        } else {
            *current_buf = *operand_pos;
            current_buf++;
            operand_pos++;
        }
    }
    *current_buf = '\0';

}