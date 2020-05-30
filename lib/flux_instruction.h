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
    ISUB,
    IMUL,
    IDIV,
    SPUSH,

    // Defines a new Exception
    // which is valid from_instruction up to to_instruction for the specified exception_type 
    // in case an exception is thrown the programm execution is continued at jump_location
    // Params:
    //  int from_instruction
    //  int to_instruction
    //  int jump_location
    //  int exception_type
    //
    //  char bytecode[] = {
    //      AEX,
    //      0,0,0,10, // from_instruction 10
    //      0,0,0,15, // up to_instruction 15
    //      0,0,0,40, // jump_location
    //      0,0,0,0   // exception_type (in this case division_by_zero)
    //  };
    AEX,

    // Instruction to throw the value which is on top of the stack
    // THROW will check the type of the thrown variable and look for a
    // appropriate Exception definition inside the ExceptionTable
    // Throw does not take any parameters
    THROW, 

} FluxInstruction;

#endif //FLUX_INSTRUCTION_H
