#include <curses.h>
#include <stdlib.h>

#include "emulator.h"
#include "assembler/instruction/instructions.h"


WINDOW * LED_window;
WINDOW * information_window;
WINDOW * status_window;
WINDOW * control_window;
WINDOW * instruction_window;
enum running_status running_operation_status;
void init_display() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    LED_window = newwin(9, 70, 0, 0);
    start_color();
    init_pair(5, COLOR_WHITE, -1);
    use_default_colors();
    bkgd(COLOR_PAIR(5));

    init_pair(1, COLOR_WHITE, COLOR_RED);    // LED ON
    init_pair(2, COLOR_WHITE, 8);            // LED OFF
    init_pair(3, COLOR_BLACK, COLOR_WHITE);  // information color
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    wbkgd(LED_window, COLOR_PAIR(4));

    status_window = newwin(1, 70, 9, 0);
    wbkgd(status_window, COLOR_PAIR(3));
    wrefresh(status_window);

    information_window = newwin(7, 70, 11, 0);
    wbkgd(information_window, COLOR_PAIR(-1));
    nodelay(information_window, true);
    keypad(information_window, true);

    control_window = newwin(1, 70, LINES-1, 0);
    wbkgd(control_window, COLOR_PAIR(3));
    print_control_window();
    wrefresh(control_window);

    instruction_window = newwin(4, 70, 19, 0);
    wbkgd(instruction_window, COLOR_PAIR(3));
    wrefresh(instruction_window);

    wrefresh(information_window);
    wrefresh(LED_window);
}

static void printHLine(int x, int y) {
    mvwaddstr(LED_window, x, y + 1, "  ");
}


static void printVLine(int x, int y) {
    for (int i=1; i<=2; i++) {
        mvwaddch(LED_window, x+i, y, ' ');
    }
}


void UpdateDisplay(int digit) {
#define DISPLAY_COLOR(mask)           \
    do {                           \
        if (disp & mask) {         \
            wattr_on(LED_window, COLOR_PAIR(1), NULL); \
        } else {                   \
            wattr_on(LED_window, COLOR_PAIR(2), NULL); \
        }                          \
    } while (0)

    attron(COLOR_PAIR(1));
    Byte disp = Memory_instance.Display[digit];
    int start_pos = digit * 8 + 3;
    int y = 1;

    DISPLAY_COLOR(0x1);
    printHLine(y, start_pos);
    DISPLAY_COLOR(0x2);
    printVLine(y, start_pos+3);
    DISPLAY_COLOR(0x4);
    printVLine(y+3, start_pos+3);
    DISPLAY_COLOR(0x8);
    printHLine(y+6, start_pos);
    DISPLAY_COLOR(0x10);
    printVLine(y, start_pos);
    DISPLAY_COLOR(0x20);
    printVLine(y+3, start_pos);
    DISPLAY_COLOR(0x40);
    printHLine(y+3, start_pos);
    DISPLAY_COLOR(0x80);
    mvwaddstr(LED_window, y+6, start_pos+3+2, "  ");
#undef DISPLAY_COLOR
}

int AccessViolation(EmulatorPtr ptr) {
    print_register_window();
    running_operation_status = status_violation;
    wattr_on(status_window, COLOR_PAIR(1), NULL);
    mvwprintw(status_window, 0, 0, "Access Violation Occur: Access to %llx.", ptr);
    wrefresh(status_window);
    print_control_window();
    while (wgetch(status_window) != 'q') {}
    exit(1);
}
void print_register_window() {
        mvwprintw(information_window, 0, 0,
                 "registers info:\n"
                 "B: %02x, C: %02x, D: %02x, E: %02x, H: %02x, L: %02x, A: %02x, \n"
                 "BC: %04x, DE: %04x, HL: %04x, AF: %04x, \n"
                 "SP: %04x, PC: %04x\n"
                 "\n"
                 "flags:\n"
                 "SF: %1x, ZF: %1x, HC:%1x, P/V: %1x, NF: %1x, CF: %1x",
                 Context_instance.B, Context_instance.C, Context_instance.D, Context_instance.E, Context_instance.H,
                 Context_instance.L, Context_instance.A,
                 Context_instance.BC, Context_instance.DE, Context_instance.HL, Context_instance.AF,
                 Context_instance.SP, Context_instance.PC,
                 Context_instance.SF, Context_instance.ZF, Context_instance.HC,
                 Context_instance.PV, Context_instance.NF, Context_instance.CF);
        wrefresh(information_window);
}

void print_control_window() {
    werase(control_window);
    if (running_operation_status == status_run) {
        mvwaddstr(control_window, 0, 0, "[Spc] Pause");
    } else if (running_operation_status == status_pause){
        mvwaddstr(control_window, 0, 0, "[Spc] Resume    [q] Quit  [s] Step Execute");
    } else if (running_operation_status == status_violation || running_operation_status == status_halt) {
        mvwaddstr(control_window, 0, 0, "[q] Quit");
    } else if (running_operation_status == status_step) {
        mvwaddstr(control_window, 0, 0, "[Spc] Next    [s] Exit Step Execute");
    }
    wrefresh(control_window);
}

void print_instruction_window() {
    char buf[128];
    ASM_Opcode instruction;
    int pos = Context_instance.PC - 0x8000;
    int before_pos = pos;
    wclear(instruction_window);
    find_instruction_by_code(Memory_instance.program, &pos, 10, &instruction);
    Opcode_to_ASM_str(&instruction, buf, 128);
    mvwprintw(instruction_window, 0,0, "> %04x | %s", before_pos + 0x8000,  buf);
    before_pos = pos;

    for (int i=1; i < 4; i++) {
        find_instruction_by_code(Memory_instance.program, &pos, 64, &instruction);
        Opcode_to_ASM_str(&instruction, buf, 128);
        mvwprintw(instruction_window, i,0, "  %04x | %s", before_pos + 0x8000,  buf);
        before_pos = pos;
    }
    wrefresh(instruction_window);
}