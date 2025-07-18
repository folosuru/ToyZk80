#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <util/util.h>
#include "emulator.h"

struct Memory Memory_instance;
struct Context Context_instance;

int main(int argc, char *argv[]) {
#ifdef DEBUG
    test_instruction();
#endif  // DEBUG
    FILE* file;
    fopen_s(&file, "out.hex", "r");
    int size;
    char* file_buf = loadFile(file, &size);
    BufferArea buffer = get_hextext(file_buf, size);
    memcpy(Memory_instance.program, buffer.buffer, buffer.size);
    init_display();
    init_emulator();
    print_instruction_window();
    for (int i=0; i < 8; i++) UpdateDisplay(i);
    wrefresh(LED_window);
    mainloop();
}




