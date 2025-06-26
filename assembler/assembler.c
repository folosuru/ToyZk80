#include "assembler.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instructions.h"
#define ARRAY_SIZEOF(arr) sizeof(arr) / sizeof(arr[0])

static const char* seek_instruction(struct seeking_text* seek_text, int* len_out);
static ASM_Opcode* tryCreateOperand(const char* name, int name_len, struct seeking_text* text,
                                    const struct ASM_Instruction* instruction);
static ASM_Opcode* findInstruction(struct seeking_text* text);
static int parse_number(struct seeking_text* text);

struct text_span_info {
    const char* line_start;
    int line_len;
    int line;
    int line_pos;
};

struct text_span_info get_line_info(const char* text_begin, const char* current) {
    struct text_span_info result = {text_begin, 0, 1, 1};
    const char* iter = text_begin;
    while (iter < current && *iter != '\0') {
        if (*iter == '\n') {
            iter++;
            result.line++;
            result.line_pos = 1;
            result.line_start = iter;
            continue;
        }
        result.line_pos++;
        iter++;
    }
    const char* line_end = result.line_start + result.line_pos;
    while (*line_end != '\0' && *line_end != '\n') {
        line_end++;
    }
    result.line_len = line_end - result.line_start;
    return result;
}

void show_errormessage(struct seeking_text* seek_text, const char* message, ...) {
    struct text_span_info line = get_line_info(seek_text->start, seek_text->current);
    fprintf(stderr, "line %d:%d: ", line.line, line.line_pos - 1);
    va_list ap;
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    fprintf(stderr, "\n%.*s\n%*.s^\n", line.line_len, line.line_start, line.line_pos - 1, "");
}

// return: instruction start pos
static const char* seek_instruction(struct seeking_text* seek_text, int* const len_out) {
    *len_out = 0;
    while (*seek_text->current != '\0') {
        if (isspace(*seek_text->current)) {
            seek_text->current++;
        } else {
            break;
        }
    }
    const char* word_start = seek_text->current;

    while (*seek_text->current != '\0') {
        if (*seek_text->current == ' ' || *seek_text->current == '\n' || *seek_text->current == '\r') {
            return word_start;
        }
        if ('A' <= *seek_text->current && *seek_text->current <= 'Z') {
            *len_out += 1;
            seek_text->current++;
            continue;
        } else {
            show_errormessage(seek_text, "Unexpected Character in Instruction");
            return NULL;
        }
    }
    if (*len_out == 0) {
        return NULL;
    }
    return word_start;
}

static char delimiter[] = {'(', ')', ',', '+'};

static _Bool is_in_delimiter(char c) {
    for (int i = 0; i < ARRAY_SIZEOF(delimiter); ++i) {
        if (c == delimiter[i]) {
            return true;
        }
    }
    return false;
}

static int parse_number(struct seeking_text* text) {
    skip_whitespace(text);
    const char* start = text->current;
    int len = 0;
    _Bool may_hex = false;
    while (*text->current != '\0') {
        // "12AB " or "12AB\n" or  "12AB,"  or "12ABH"...

        if (isspace(*text->current) || is_in_delimiter(*text->current) || *(text->current) == 'H' ||
            *(text->current) == 'h') {
            break;
        }

        if ('0' <= *(text->current) && *(text->current) <= '9') {
            len++;
            (text->current)++;
            continue;
        }
        if (('A' <= *(text->current) && *(text->current) <= 'F') ||
            'a' <= *(text->current) && *(text->current) <= 'f') {
            len++;
            (text->current)++;
            may_hex = true;
            continue;
        }

        text->current = start;
        return -1;
    }
    skip_whitespace(text);
    int data;
    char buffer[128];
    memcpy(buffer, start, len);
    buffer[len] = '\0';
    if (*(text->current) == 'H' || *(text->current) == 'h') {
        sscanf_s(buffer, "%x", &data);
        (text->current)++;
    } else {
        sscanf_s(buffer, "%d", &data);
        if (may_hex) {
            text->current = start;
            return -1;
        }
    }
    return data;
}

bool is_label_char(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_') || ('0' <= c && c <= '9');
}

bool is_label_first_char(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || (c == '_');
}

static const char* register_text[] = {"B",  "C",  "D", "E",  "H", "L",  "A",  "BC", "DF", "HL", "SP",
                                      "AF", "NZ", "Z", "NC", "C", "PO", "PE", "P",  "M",  "IX", "IY"};

void seek_label(struct seeking_text* text, struct ASM_Label_Data* label_out) {
    const char* start = text->current;
    skip_whitespace(text);
    label_out->length = 0;
    label_out->name = text->current;
    if (!is_label_first_char(*text->current)) {
        return;
    }
    label_out->length++;
    text->current++;

    while (is_label_char(*text->current)) {
        label_out->length++;
        text->current++;
    }

    for (int i = 0; i < ARRAY_SIZEOF(register_text); i++) {
        if (strncmp(label_out->name, register_text[i], label_out->length) == 0) {
            label_out->length = 0;
            text->current = start;
            return;
        }
    }
}

bool read_label_def(struct seeking_text* text, struct ASM_Label_Data* label_out) {
    skip_whitespace(text);
    skip_return(text);
    const char* start = text->current;
    if (*text->current == '\0') {
        goto wrong_return;
    }

    seek_label(text, label_out);
    if (label_out->length == 0) {
        goto wrong_return;
    }
    skip_whitespace(text);
    if (*text->current == ':') {
        text->current++;
        return true;
    }

wrong_return: {
    text->current = start;
    return false;
}
}

static ASM_Opcode* tryCreateOperand(const char* name, int name_len, struct seeking_text* text,
                                    const struct ASM_Instruction* instruction) {
    if (strncmp(instruction->instruction, name, name_len) != 0) {
        return NULL;
    }

    const char* start = text->current;
    const char* operand_text = instruction->operand;
    struct Opcode_data data[6];
    int data_cnt = 0;

    //  v  -> v seek here
    // text:    "      A,"
    // operand:       "A,B"
    skip_whitespace(text);

    // delimiterの前後=space ok
    while (*operand_text != '\0' && *text->current != '\0') {
        if (is_in_delimiter(*operand_text)) {
            //  v  -> v seek here
            // text: "      ,A"
            // operand:    ",A"
            skip_whitespace(text);

            if (*operand_text != *text->current) {
                goto wrong_end;
            }
            operand_text++;
            (text->current)++;

            //  v -> v seek here
            // text: ",     A"
            // operand:    "A"
            skip_whitespace(text);
            continue;
        }

        if (*operand_text == 'n' || *operand_text == 'd') {
            operand_text++;

            _Bool is_16bit = false;
            if (*operand_text == 'n') {
                operand_text++;
                is_16bit = true;
            }

            int number = parse_number(text);
            if (number == -1) {
                if (is_16bit) {
                    struct ASM_Label_Data label;
                    seek_label(text, &label);
                    if (label.length != 0) {
                        if (*text->current == ':') {
                            text->current++;
                        }
                        data[data_cnt].type = Opcode_data_2byte;
                        data[data_cnt].label = label;
                        data_cnt++;
                        skip_whitespace(text);
                        continue;
                    }
                }
                goto wrong_end;
            }

            data[data_cnt].data = number;
            if (is_16bit) {
                data[data_cnt].type = Opcode_data_2byte;
            } else {
                data[data_cnt].type = Opcode_data_1byte;
            }
            data_cnt++;
            skip_whitespace(text);
            continue;
        }
        if (*operand_text == 'e') {
            operand_text++;
            struct ASM_Label_Data label;
            seek_label(text, &label);
            if (label.length == 0) {
                return NULL;
            }
            data[data_cnt].label = label;
            data[data_cnt].type = Opcode_data_label_relative;
            data_cnt++;
            continue;
        }

        if (*operand_text != *text->current) {
            goto wrong_end;
        }

        text->current++;
        operand_text++;
    }

    skip_whitespace(text);
    if (!skip_return(text) && *text->current != '\0') {
        goto wrong_end;
    }

    ASM_Opcode* result = malloc(sizeof(ASM_Opcode));
    result->instruction = instruction;
    result->data_len = data_cnt;
    for (int i = 0; i < data_cnt; i++) {
        result->data[i] = data[i];
    }
    return result;

wrong_end: {
    text->current = start;
    return NULL;
}
}

static ASM_Opcode* findInstruction(struct seeking_text* text) {
    int name_len;
    const char* instruction_start = text->current;

    const char* name = seek_instruction(text, &name_len);
    if (name == NULL) {
        text->current = instruction_start;
        return NULL;
    }

    for (int i = 0; i < ASM_Instructions_count; i++) {
        ASM_Opcode* op = tryCreateOperand(name, name_len, text, &ASM_instructions[i]);
        if (op != NULL) {
            return op;
        }
    }

    show_errormessage(text, "not match any instructions");
    text->current = instruction_start;
    return NULL;
}

BufferArea assemble(const char* source, struct Command_flags* flag) {
    struct seeking_text s = {source, source};
    int line_count = 0;
    while (*s.current != '\0') {
        if (*s.current == '\n') {
            line_count++;
        }
        s.current++;
    }
    s.current = s.start;

    BufferArea result;
    result.buffer = calloc(sizeof(char) * 4 * line_count, 0);
    result.size = 0;

    struct unresolved_label unresolvedLabel[128];
    int unresolvedLabel_cnt = 0;

    struct defined_label definedLabel[128];
    int definedLabel_count = 0;

    unsigned char* buffer_current = result.buffer;
    while (*s.current != '\0') {
        struct ASM_Label_Data label_tmp;
        if (read_label_def(&s, &label_tmp)) {
            definedLabel[definedLabel_count].name = label_tmp.name;
            definedLabel[definedLabel_count].name_len = label_tmp.length;
            definedLabel[definedLabel_count].position = buffer_current;
            definedLabel_count++;
            continue;
        }

        ASM_Opcode* p = findInstruction(&s);
        if (p == NULL) {
            // skip line
            struct text_span_info info = get_line_info(s.start, s.current);
            s.current = info.line_start + info.line_len;
            skip_whitespace(&s);
            continue;
        }

        unsigned char* next_instruction_pos = buffer_current + getOpcodeSize(p->instruction);

        for (int i = 0; i < p->instruction->OpCodeCount; i++) {
            *buffer_current = p->instruction->OpCode[i];
            buffer_current++;
        }

        for (int i = 0; i < p->data_len; i++) {
            switch (p->data[i].type) {
                case Opcode_data_1byte: {
                    *buffer_current = (p->data[i].data & 0xFF);
                    buffer_current++;
                    break;
                }
                case Opcode_data_2byte: {
                    *buffer_current = (p->data[i].data & 0xFF);
                    buffer_current++;

                    *buffer_current = ((p->data[i].data >> 8) & 0xFF);
                    buffer_current++;
                    break;
                }
                case Opcode_data_label_relative: {
                    if (unresolvedLabel_cnt >= ARRAY_SIZEOF(unresolvedLabel)) {
                        fprintf(stderr, "label is too many");
                        return result;
                    }
                    unresolvedLabel[unresolvedLabel_cnt].name = p->data[i].label.name;
                    unresolvedLabel[unresolvedLabel_cnt].name_len = p->data[i].label.length;
                    unresolvedLabel[unresolvedLabel_cnt].write_position = buffer_current;
                    unresolvedLabel[unresolvedLabel_cnt].next_instruction = next_instruction_pos;
                    unresolvedLabel[unresolvedLabel_cnt].type = unresolved_label_type_relative;

                    unresolvedLabel_cnt++;
                    buffer_current++;
                    break;
                }
                case Opcode_data_label_absolute: {
                    if (unresolvedLabel_cnt >= ARRAY_SIZEOF(unresolvedLabel)) {
                        fprintf(stderr, "label is too many");
                        return result;
                    }
                    unresolvedLabel[unresolvedLabel_cnt].name = p->data[i].label.name;
                    unresolvedLabel[unresolvedLabel_cnt].name_len = p->data[i].label.length;
                    unresolvedLabel[unresolvedLabel_cnt].write_position = buffer_current;
                    unresolvedLabel[unresolvedLabel_cnt].next_instruction = NULL;
                    unresolvedLabel[unresolvedLabel_cnt].type = unresolved_label_type_absolute;
                    unresolvedLabel_cnt++;
                    buffer_current += 2;
                    break;
                }
                case Opcode_data_offset:
                    break;
            }
        }
    }
    result.size = buffer_current - result.buffer;

    for (int i = 0; i < unresolvedLabel_cnt; i++) {
        int label_pos = resolveLabel(&unresolvedLabel[i], &definedLabel, definedLabel_count);
        if (label_pos == -1) {
            struct seeking_text t = {source, unresolvedLabel[i].name};
            show_errormessage(&t, "definition of label %.*s is  not found", unresolvedLabel[i].name_len,
                              unresolvedLabel[i].name);
            continue;
        }
        struct defined_label* found_label = &definedLabel[label_pos];

        if (unresolvedLabel[i].type == unresolved_label_type_relative) {
            int addr_diff = found_label->position - unresolvedLabel[i].next_instruction;

            if (!(SCHAR_MIN <= addr_diff && addr_diff <= SCHAR_MAX)) {
                // addr diff too far
                struct seeking_text t = {source, unresolvedLabel[i].name};
                show_errormessage(&t, "label %.*s is too far", found_label->name_len, found_label->name);
                continue;
            }

            *(unresolvedLabel[i].write_position) = (signed char)(addr_diff);
        } else {
            int label_absolute_pos = found_label->position - result.buffer + flag->start_address;
            unresolvedLabel[i].write_position[0] = 0xFF & (label_absolute_pos);
            unresolvedLabel[i].write_position[1] = (0xFF00 & (label_absolute_pos)) >> 8;
        }
    }

    return result;
}

void print_BufferArea(BufferArea area, FILE* out) {
    for (int i = 0; i < area.size; i++) {
        fprintf(out, "%02x", area.buffer[i]);
    }
}
