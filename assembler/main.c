#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

extern void print_help();

int main(int argc, char* argv[]) {
    FILE* file;
    struct Command_flags flags;
    flags.create_addressJump_label = false;
    flags.show_assemble_address = false;
    flags.start_address = 0x8000;

    bool show_help = false;

    // 0=> assemble
    // 1=> disassemble
    int mode = 0;

    const char* filename = NULL;
    const char* output_name = NULL;
    for (int i = 1; i < argc; ++i) {
        if (strcmp("--assemble", argv[i]) == 0) {
            mode = 0;
            continue;
        }
        if (strcmp("--disassemble", argv[i]) == 0) {
            mode = 1;
            continue;
        }
        if (strcmp("--label", argv[i]) == 0) {
            flags.create_addressJump_label = true;
            continue;
        }
        if (strcmp("--address", argv[i]) == 0) {
            flags.show_assemble_address = true;
            continue;
        }
        if (strcmp("--help", argv[i]) == 0) {
            show_help = true;
            continue;
        }
        if (strcmp("-o", argv[i]) == 0) {
            if (i + 1 < argc) {
                i++;
                output_name = argv[i];
            }
            continue;
        }
        if (strcmp("-start", argv[i]) == 0) {
            if (i + 1 < argc) {
                i++;
                sscanf_s(argv[i], "%x", &flags.start_address);
            }
            continue;
        }
        filename = argv[i];
    }
    if (filename == NULL) {
        printf("no input");
        print_help();
        exit(1);
    }

    fopen_s(&file, filename, "r");
    int size;
    char* buf = loadFile(file, &size);

    if (output_name != NULL) {
        fopen_s(&flags.out, output_name, "w");
    } else {
        flags.out = stdout;
    }

    if (mode == 0) {
        print_BufferArea(assemble(buf, &flags), flags.out);
    } else {
        disassemble(buf, size, &flags);
    }
    if (output_name != NULL) fclose(flags.out);
}

void print_help() {
    printf(
        "ToyZk80_asm - assembler\n"
        "Usage: ToyZk80_asm input [-o output_file] [-start start_address] [--assemble] [--disassemble] [--label]\n");
}
