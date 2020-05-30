#ifndef FLUX_INSTRUCTION_H
#define FLUX_INSTRUCTION_H

typedef enum {

    // Add the two integers which are on top of the stack together and put the result back on the stack
    IADD = 1,

    // Convert the Integer on the stack to a double
    ITOD,

    // Push an integer to the stack
    // Params:
    //  int number
    //
    //  char bytecode[] = {
    //      IPUSH,
    //      0,0,0,10 
    //  };
    IPUSH,

    // Pop a value from the stack
    POP,

    // Load a variable from index onto the stack
    // Params:
    //  int index
    //
    //  // load variable from index 1
    //  char bytecode[] = {
    //      LOAD,
    //      0,0,0,1
    //  };
    LOAD,

    // Opposite of LOAD.
    // Store Variable at given index and remove it from the stack
    // Params:
    //  int index
    //
    //  // Store value at index 3
    //  char bytecode[] = {
    //      STORE,
    //      0,0,0,3
    //  };
    STORE,

    // PRINT whatever is on the stack
    PRINT,

    // JumpSubRoutine - jumps uncoditionally to given address and puts the return address on the stack
    // Params:
    //  Int ReturnAddress
    //
    //  char bytecode[] = {
    //      JSR,
    //      0,0,0,20
    //  };
    JSR,

    // Compare the top two elements of the stack.
    // removes the elements from the stack
    // Sets cmp_flag which is used by the various conditional Jump instructions
    CMP,

    // Jump to given address if cmp_flag is set to GREATER
    JG,

    // Jump to given address if cmp_flag is set to EQUAL
    JE,
    // Jump to given address if cmp_flag is set to LESS
    JL,

    // Jump to given address if cmp_flag is set to GREATER or EQUAL
    JGE,
    // Jump to given address if cmp_flag is set to LESS or EQUAL
    JLE,

    // Inspect a given variable - right now its like print
    INSPECT,

    // subtract two integers from each other and put the result on the stack
    ISUB,

    // multiply two integers and put the result on the stack
    IMUL,

    // divide two integers and put the result on the stack
    IDIV,

    // push a String on the stack
    // Params:
    //  int str_len
    //  char[] string
    //
    //  char bytecode[] = {
    //      SPUSH,
    //      0,0,0,6
    //      'H','e','l','l','o','\0'
    //  };
    //
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

    // return from function call
    RET,

    // Exit the program
    // Does not take any arguments at the moment
    EXIT,

} FluxInstruction;

#endif //FLUX_INSTRUCTION_H
