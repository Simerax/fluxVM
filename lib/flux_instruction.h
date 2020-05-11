#ifndef FLUX_INSTRUCTION_H
#define FLUX_INSTRUCTION_H

typedef enum {
    IADD = 1,
    ITOD,
    IPUSH,
    POP,
    LOAD,
    STORE,
    PRINT
} flux_instruction;

#endif //FLUX_INSTRUCTION_H
