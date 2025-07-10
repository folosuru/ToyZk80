#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static int is_return_char(const char* text) {
    if (*text == '\r') {
        if (*(text + 1) == '\n') {  // CRLF
            return 2;
        }
        return 1;  // CR
    }
    if (*text == '\n') {
        return 1;  // LF
    }
    return 0;  // not return;
}

_Bool skip_whitespace(struct seeking_text* text) {
    bool success = false;
    if (isblank(*text->current)) {
        do {
            (text->current)++;
        } while (isblank(*text->current));
        success = true;
    }
    if (*text->current == ';') {
        do {
            text->current++;
        } while (is_return_char(text->current) == 0);
        success = true;
    }
    return success;
}

_Bool skip_return(struct seeking_text* text) {
    int skip = is_return_char(text->current);
    if (skip == 0) {
        return false;
    }
    do {
        skip = is_return_char(text->current);
        text->current += skip;
    } while (skip != 0);
    return true;
}


void print_BufferArea(BufferArea area, FILE* out) {
    for (int i = 0; i < area.size; i++) {
        fprintf(out, "%02x", area.buffer[i]);
    }
}

enum is_hex_result {
    hex_char_none,
    hex_char_number,
    hex_char_alpha
};
static enum is_hex_result is_hex_char(char c) {
    if ('0' <= c && c <= '9') {
        return hex_char_number;
    }
    if (('A' <= c && c <= 'F') || ('a' <= c && c <= 'f')) {
        return hex_char_alpha;
    }
    return hex_char_none;
}

static bool parse_hex_char(const char* c1, const char* c2, unsigned char* result) {
    enum is_hex_result c1_result = is_hex_char(*c1);
    if (c1_result == hex_char_number) {
        *result |= (*c1 - '0') << 7;
    } else if (c1_result == hex_char_alpha) {
        *result |= (tolower(*c1) - 'a' +10) << 7;
    } else {
        return false;
    }
    enum is_hex_result c2_result = is_hex_char(*c2);
    if (c2_result == hex_char_number) {
        *result |= (*c2 - '0');
    } else if (c2_result == hex_char_alpha) {
        *result |= (tolower(*c2) - 'a' +10);
    } else {
        return false;
    }
    return true;
}

BufferArea get_hextext(const char* source, int source_len) {
    struct seeking_text seek_src = {source, source};
    unsigned char* data_area;
    int data_len = 0;
    data_area = malloc(source_len / 2);
    if (data_area == NULL) {
        exit(0);
    }
    skip_whitespace(&seek_src);
    skip_return(&seek_src);

    while ((seek_src.current - seek_src.start) < source_len - 2) {
        unsigned char v;
        if (!parse_hex_char(seek_src.current, seek_src.current+1, &v)) {
            fprintf(stderr, "unexpected character in source\n");
            exit(0);
        }

        data_area[data_len] = v;
        seek_src.current += 2;
        data_len++;

        while (true) {
            if (!skip_whitespace(&seek_src) && !skip_return(&seek_src)) break;
        }
    }
    BufferArea result = {data_area, data_len};
    return result;
}

char* loadFile(FILE* file, int* size) {
    if (file == 0) {
        fprintf(stderr, "file load err");
        return 0;
    }
    if (fseek(file, 0, SEEK_END) == -1) {
        fprintf(stderr, "file load err: file cannot seek.");
        return 0;
    }
    *size = ftell(file);
    if (fseek(file, 0, SEEK_SET) == -1) {
        fprintf(stderr, "file load err:file cannot seek.");
        return 0;
    }
    char* buf = calloc(1, (*size) + 2);
    fread(buf, *size, 1, file);
    fclose(file);
    if (size == 0 || buf[(*size) - 1] != '\n') {
        buf[(*size)++] = '\n';
    }
    return buf;
}