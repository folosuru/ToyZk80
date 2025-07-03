#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "emulator.h"

struct {
    uint8_t fooo;
} d;


struct Memory Memory_instance;
struct Context Context_instance;
void test();

int main(int argc, char *argv[]) {
    test_instruction();

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();			/* Start color */
    init_pair(1, COLOR_WHITE, COLOR_RED);  //LED ON
    init_pair(2, COLOR_WHITE, 8 );   // LED OFF

    init_emulator();
    Byte program[] = {
                      0x3e,
                      0x00,
                      0x21,
                      0xf8,
                      0x83,
                      0x06,
                      0x00,
                      0x77,
                      0x00,
                      0x10,
                      0xfc,
                      0x3c,
                      0xc2,
                      0x05,
                      0x80,
                      0x2c,
                      0xc2,
                      0x05,
                      0x80,
                      0xc3,
                      0x02,
                      0x80,
                      0x76
    };
    memcpy(Memory_instance.program, &program, sizeof(program));
    for (int i=0; i < 8; i++) UpdateDisplay(i);
    mainloop();
}




