#ifndef FLUX_EXCEPTION_H
#define FLUX_EXCEPTION_H

typedef struct {
    unsigned int from_instruction;
    unsigned int to_instruction;
    unsigned int jump_instruction;

    char* type_name; 
    unsigned int type_name_length;
} FluxException;


FluxException* flux_exception_init(unsigned int from, unsigned int to, unsigned int jump, char* type, unsigned int type_str_length);
void flux_exception_free(FluxException*);

#endif
