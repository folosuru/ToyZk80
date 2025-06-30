#include <curses.h>

#define UNIMPLEMENT_DEF(name) void name() { \
mvprintw(24, 5, #name "is not implemented (yet)");            \
    }

#include "unimplement_instruction.h"
