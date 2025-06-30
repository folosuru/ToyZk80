#include <curses.h>
#include <stdlib.h>

#include "emulator.h"



static void printHLine(int x, int y) {
    mvaddstr(x, y + 1, "  ");
}


static void printVLine(int x, int y) {
    for (int i=1; i<=2; i++) {
        mvaddch(x+i, y, ' ');
    }
}


void UpdateDisplay(int digit) {
#define DISPLAY_COLOR(mask)           \
    do {                           \
        if (disp & mask) {         \
            attron(COLOR_PAIR(1)); \
        } else {                   \
            attron(COLOR_PAIR(2)); \
        }                          \
    } while (0)

    attron(COLOR_PAIR(1));
    Byte disp = Memory_instance.Display[digit];
    int start_pos = digit * 8;

    DISPLAY_COLOR(0x1);
    printHLine(5, start_pos);
    DISPLAY_COLOR(0x2);
    printVLine(5, start_pos+3);
    DISPLAY_COLOR(0x4);
    printVLine(5+3, start_pos+3);
    DISPLAY_COLOR(0x8);
    printHLine(5+6, start_pos);
    DISPLAY_COLOR(0x10);
    printVLine(5, start_pos);
    DISPLAY_COLOR(0x20);
    printVLine(5+3, start_pos);
    DISPLAY_COLOR(0x40);
    printHLine(5+3, start_pos);
    DISPLAY_COLOR(0x80);
    mvaddstr(5+6, start_pos+3+2, "  ");
#undef DISPLAY_COLOR
}

int AccessViolation(EmulatorPtr ptr) {
    char buf[512];
    sprintf(buf, "Access Violation Occur at %llx.", ptr);
    mvaddstr(20, 5, buf);
    mvprintw(24, 5, "PC: %x", Context_instance.PC);
    getch();
    exit(2);
}