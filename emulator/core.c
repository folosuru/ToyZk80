
#include <synchapi.h>
#include <unistd.h>

#include "curses.h"
#include "emulator.h"


void init_emulator() {
    Context_instance.PC = 0x8000;
    Context_instance.SP = 0x81ff;
    return;
}

_Noreturn void mainloop() {
    const InstructionPtrTable table = InstructionTable();

    running_operation_status = status_run;
    mvwaddstr(status_window, 0, 0, "program running");
    wrefresh(status_window);
    print_register_window();
    while (true) {
        setStatus(status_pause);

        while (true) {
            int op = wgetch(status_window);
            if (op == ' ') {
                setStatus(status_run);
                run_loop(table);
                break;
            }
            if (op == 's') {
                setStatus(status_step);
                step_run(table);
                break;
            }
            if (op == 'q') {
                exit(0);
            }
        }
    }
}
void run_loop(const InstructionPtrTable table) {
    while (true) {
        if (wgetch(information_window) == ' ') {
            return;
        }
        Byte current_PC = MemoryManager_ByteRead(Context_instance.PC);
        if (current_PC == 0x00) {  // NOP
            if (!(Context_instance.R & 0xFF)) {
                print_register_window();
                Sleep(1);
            }
            Context_instance.PC++;
        } else {
            (*(*table)[current_PC])();
        }
        Context_instance.R++;
    }
}

void step_run(const InstructionPtrTable table) {
    while (true) {
        while (true) {
            int ch = wgetch(status_window);
            if (ch == 's') {
                return;
            }
            if (ch == ' ') {
                break;
            }
        }
        Byte current_PC = MemoryManager_ByteRead(Context_instance.PC);
        if (current_PC == 0x00) {  // NOP
            Context_instance.PC++;
        } else {
            (*(*table)[current_PC])();
        }
        Context_instance.R++;

        print_register_window();
        print_instruction_window();
    }
}

void setStatus(const enum running_status new_stat) {
    running_operation_status = new_stat;
    switch (running_operation_status) {
        case status_run:
            mvwaddstr(status_window, 0, 0, "program running");
            break;
        case status_pause:
            mvwaddstr(status_window, 0, 0, "program paused.");
            break;
        case status_violation:
            break;
        case status_halt:
            break;
        case status_step:
            mvwaddstr(status_window, 0, 0, "Step Execution Mode");
            break;
    }
    print_control_window();
    wrefresh(status_window);
}

Byte MemoryManager_ByteRead(EmulatorPtr ptr) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff) {
            return Memory_instance.program[ptr - 0x8000];
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            return Memory_instance.Display[ptr - 0x83f8];
        }
    }
    return AccessViolation(ptr);
}

Word MemoryManager_WordRead(EmulatorPtr ptr) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff) {
            return *(Word*)(&Memory_instance.program[ptr - 0x8000]);
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            return *(Word*)(&Memory_instance.Display[ptr - 0x83f8]);
        }
    }
    return AccessViolation(ptr);
}

void MemoryManager_ByteWrite(EmulatorPtr ptr, Byte data) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff) {
            *(Byte*)(&Memory_instance.program[ptr - 0x8000]) = data;
            return;
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            *(Byte*)(&Memory_instance.Display[ptr - 0x83f8]) = data;
            UpdateDisplay(ptr - 0x83f8);
            wrefresh(LED_window);
            return;
        }
    }
    AccessViolation(ptr);
}

void MemoryManager_WordWrite(EmulatorPtr ptr, Word data) {
    if (0x8000 <= ptr) {
        if (ptr <= 0x81ff) {
            *(Word*)(&Memory_instance.program[ptr - 0x8000]) = data;
            return;
        }
        if (0x83f8 <= ptr && ptr <= 0x83ff) {
            *(Word*)(&Memory_instance.Display[ptr - 0x83f8]) = data;
            UpdateDisplay(ptr - 0x83f8);
            UpdateDisplay(ptr - 0x83f8 + 1);
            wrefresh(LED_window);
            return;
        }
    }
    AccessViolation(ptr);
}
