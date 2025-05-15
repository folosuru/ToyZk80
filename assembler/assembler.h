#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef struct BufferArea_ {
    char* buffer;
    int size;
} BufferArea;

BufferArea assemble(char* source);
#endif  // ASSEMBLER_H
