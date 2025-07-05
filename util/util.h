#ifndef TOYTK80_UTIL_H
#define TOYTK80_UTIL_H
#include <stdio.h>
typedef struct BufferArea_ {
    unsigned char* buffer;
    int size;
} BufferArea;

struct seeking_text {
    const char* const start;
    const char* current;
};

void print_BufferArea(BufferArea area, FILE* out);
_Bool skip_whitespace(struct seeking_text* text);
_Bool skip_return(struct seeking_text* text);
BufferArea get_hextext(const char* text, int len);
char* loadFile(FILE* file, int* size);
#endif  // TOYTK80_UTIL_H
