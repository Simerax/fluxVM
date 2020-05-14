#ifndef FLUX_INSTRUCTION_H
#define FLUX_INSTRUCTION_H

typedef enum {
    IADD = 1,
    ITOD,
    IPUSH,
    POP,
    LOAD,
    STORE,
    PRINT,
    JSR,
    CMP,
    JG,
    JE,
    JL,
    JGE,
    JLE,
    INSPECT,

} FluxInstruction;

#endif //FLUX_INSTRUCTION_H
