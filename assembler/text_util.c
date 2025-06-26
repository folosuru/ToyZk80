#include "assembler.h"
#include "stdbool.h"
#include "ctype.h"

_Bool skip_whitespace(struct seeking_text* text) {
    if (isblank(*text->current)) {
        do {
            (text->current)++;
        } while (isblank(*text->current));
        return true;
    }
    return false;
}
int is_return_char(const char* text) {
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
    do {
        skip = is_return_char(text->current);
        text->current += skip;
    } while(skip != 0);
    return true;
}
