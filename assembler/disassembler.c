#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "assembler.h"

#define ARRAY_SIZEOF(arr) sizeof(arr) / sizeof(arr[0])

static int compare_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

static void append_label_list(int (*append_list)[], int* list_current, int add) {
    for (int j = 0; j < *list_current; j++) {
        if ((*append_list)[j] == add) {
            return;
        }
    }
    (*append_list)[*list_current] = add;
    (*list_current)++;
}

void disassemble(const char* source, int source_len, struct Command_flags* flags) {
    FILE* out = flags->out;

    BufferArea buf = get_hextext(source, source_len);
    int data_len = buf.size;
    unsigned char* data_area = buf.buffer;

    int seek_pos = 0;
    int program_pos = flags->start_address;

    int unnamed_label_pos[1024];
    int unnamed_label_pos_cnt = 0;
    ASM_Opcode opcode_list[1024];
    int opcode_list_cnt = 0;

    while (seek_pos < data_len) {
        opcode_list[opcode_list_cnt].data_len = 0;
        find_instruction_by_code(data_area, &seek_pos, data_len, &opcode_list[opcode_list_cnt]);
        program_pos += getOpcodeSize(opcode_list[opcode_list_cnt].instruction);

        for (int i = 0; i < ARRAY_SIZEOF(opcode_list[opcode_list_cnt].data); i++) {
            if (opcode_list[opcode_list_cnt].data[i].type == Opcode_data_offset) {
                int label_pos = program_pos + opcode_list[opcode_list_cnt].data[i].s1bit_data;
                if (unnamed_label_pos_cnt < ARRAY_SIZEOF(unnamed_label_pos)) {
                    append_label_list(&unnamed_label_pos, &unnamed_label_pos_cnt, label_pos);
                } else {
                    fprintf(stderr, "label is too many");
                }
            } else if (opcode_list[opcode_list_cnt].data[i].type == Opcode_data_2byte &&
                       flags->create_addressJump_label) {
                if (unnamed_label_pos_cnt < ARRAY_SIZEOF(unnamed_label_pos)) {
                    int may_addr = opcode_list[opcode_list_cnt].data[i].data;
                    if (flags->start_address <= may_addr && may_addr <= flags->start_address + data_len) {
                        append_label_list(&unnamed_label_pos, &unnamed_label_pos_cnt, may_addr);
                    }
                } else {
                    fprintf(stderr, "label is too many");
                }
            }
        }
        opcode_list_cnt++;
    }

    program_pos = flags->start_address;
    qsort(unnamed_label_pos, unnamed_label_pos_cnt, sizeof(int), compare_int);

    int current_label_index = 0;
    for (int i = 0; i < opcode_list_cnt; ++i) {
        if (program_pos == unnamed_label_pos[current_label_index]) {
            if (flags->show_assemble_address) {
                fprintf(out, "0x%.4x | ", program_pos);
            }
            fprintf(out, "LABEL_%d: \n", current_label_index);
            if (current_label_index < unnamed_label_pos_cnt) current_label_index++;
        }
        ASM_Opcode* current = &opcode_list[i];
        if (flags->show_assemble_address) {
            fprintf(out, "0x%.4x | ", program_pos);
        }
        program_pos += getOpcodeSize(current->instruction);


        fprintf(out, "%*s%s ",4,"", current->instruction->instruction);
        const char* current_operand_char = current->instruction->operand;
        int current_operand_len = 0;
        int current_operand_data_count = 0;
        while (*current_operand_char != '\0') {
            if (*current_operand_char == 'n' || *current_operand_char == 'e') {
                fprintf(out, "%.*s", current_operand_len, current_operand_char - current_operand_len);
                current_operand_len = 0;

                if (*(current_operand_char) == 'n') {
                    if (*(current_operand_char + 1) == 'n') {
                        int value = current->data[current_operand_data_count].data;

                        if ((flags->start_address <= value && value <= flags->start_address + data_len &&
                             flags->create_addressJump_label)) {
                            int found_label_number = -1;
                            for (int j = 0; j < unnamed_label_pos_cnt; j++) {
                                if (unnamed_label_pos[j] == value) {
                                    found_label_number = j;
                                    break;
                                }
                            }
                            if (found_label_number != -1) {
                                fprintf(out, "LABEL_%d", found_label_number);
                                current_operand_char += 2;
                                continue;
                            }
                        }
                        fprintf(out, "%04xH", (current->data[current_operand_data_count].data));
                        current_operand_char += 2;
                        continue;
                    } else {
                        fprintf(out, "%02xH", (current->data[current_operand_data_count].data));
                        current_operand_char++;
                        continue;
                    }
                }
                if (*(current_operand_char) == 'e') {
                    int found_label_number = -1;
                    for (int j = 0; j < unnamed_label_pos_cnt; j++) {
                        if (unnamed_label_pos[j] ==
                            current->data[current_operand_data_count].s1bit_data + program_pos) {
                            found_label_number = j;
                            break;
                        }
                    }
                    fprintf(out, "LABEL_%d", found_label_number);
                    current_operand_char += 1;
                    continue;
                }

                current_operand_char++;
                continue;
            }
            current_operand_len++;
            current_operand_char++;
        }

        fprintf(out, "%.*s", current_operand_len, current_operand_char - current_operand_len);
        fprintf(out, "\n");
    }
}
