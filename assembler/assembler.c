#include "assembler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "instructions.h"

typedef const char* un_writeable_text;
typedef un_writeable_text* const un_writeable_text_ptr;

struct text_span_info {
    const char* line_start;
    int line_len;
    int line;
    int line_pos;
};

struct text_span_info get_line_info(const char* text_begin, const char* current) {
    struct text_span_info result = {text_begin, 0, 1,1};
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
static _Bool is_whitespace(const char* text) {
    return (*text != '\0' && (*text == ' ' || *text == '\t'));
}
static _Bool skip_whitespace(struct seeking_text* text) {
    if (is_whitespace(text->current)) {
        do {
            (text->current)++;
        } while (is_whitespace(text->current));
        return true;
    }
    return false;
}
static int is_return_char(const char* text) {
    if (*text == '\r') {
        if (*(text+1) == '\n') { // CRLF
            return 2;
        }
        return 1; // CR
    }
    if (*text == '\n') {
        return 1;  // LF
    }
    return 0;  // not return;
}

_Bool skip_return(struct seeking_text* text) {
    int skip = is_return_char(text->current);
    if (skip == 0) {
        return false;
    }
    text->current += skip;
    return true;
}

// return: instruction start pos
const char* seek_instruction(struct seeking_text* seek_text, int* const len_out) {
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
        if (*seek_text->current == ' ') {
            return word_start;
        }
            if ('A' <= *seek_text->current && *seek_text->current <= 'Z') {
                *len_out += 1;
                seek_text->current++;
                continue;
            } else {
                struct text_span_info line = get_line_info(seek_text->start, seek_text->current);
                printf("line %d:%d: Unexpected Character\n", line.line, line.line_pos-1);
                printf("%.*s\n%*.s^\n", line.line_len , line.line_start, line.line_pos-1, "" );
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
    for (int i=0; i < sizeof(delimiter)/sizeof(delimiter[0]); ++i) {
        if (c == delimiter[i]) {
            return true;
        }
    }
    return false;
}

void parse_number():;

ASM_Opcode *tryCreateOperand(const char* name, int name_len,
                             struct seeking_text* text, const struct ASM_Instruction* instruction) {

    if (strncmp(instruction->instruction, name, name_len) != 0) {
        return NULL;
    }

    const char* start = text->current;
    const char* operand_text = instruction->operand;
    char data[4];

    //           v  -> v seek here
    // text:    "      A,"
    // operand:       "A,B"
    skip_whitespace(text);

    // delimiterの前後=space ok
    while (*operand_text != '\0' && *text->current != '\0') {
        if (is_in_delimiter(*operand_text)) {
            //        v  -> v seek here
            // text: "      ,A"
            // operand:    ",A"
            skip_whitespace(text);

            if (*operand_text != *text->current) {
                goto wrong_end;
            }
            operand_text++;
            (text->current)++;

            //         v -> v seek here
            // text: ",     A"
            // operand:    "A"
            skip_whitespace(text);
            continue;
        }

        if (*operand_text == 'n') {
            operand_text++;
            if (*operand_text == 'n') {
                operand_text++;
            }


        }

        if (*operand_text != *text->current) {
            goto wrong_end;
        }

        text->current++;
        operand_text++;
    }
    skip_whitespace(text);
    if (!skip_return(text)) {
        printf("%s %s: nanka aru\n", instruction->instruction, instruction->operand);
        goto wrong_end;
    }

    ASM_Opcode* result = malloc(sizeof(ASM_Opcode));
    result->instruction = instruction;
    result->data_len = 0;
    result->data[0] = 0;
    return result;

    wrong_end: {
        text->current = start;
        return NULL;
    }
    int a_data = 0xFF;
    const char* a_start = "12FF12";
    int len = 4;
    char buffer[128];
    memcpy(buffer, start, len);
    buffer[len] = '\0';
    sscanf(buffer, "%x", &data);
    printf("%x", data);
}

ASM_Opcode* findInstruction(struct seeking_text* text) {
    int name_len;
    const char* name = seek_instruction(text, &name_len);
    for (int i=0; i < ASM_Instructions_count; i++) {
        ASM_Opcode* op = tryCreateOperand(name, name_len, text, &ASM_instructions[i]);
        if (op != NULL) {
            return op;
        }
    }
    return NULL;
}


BufferArea assemble(char* source) {
    struct seeking_text s = {source, source};
    ASM_Opcode * p =findInstruction(&s);
    printf("%x", p->instruction->OpCode[0]);

    BufferArea result;
    result.buffer = malloc(sizeof(char)* 512);
    result.size = 0;



    return result;
}
