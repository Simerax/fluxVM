#ifndef FLUX_EXCEPTION_H
#define FLUX_EXCEPTION_H

/*
 * List of builtin exception types
 * values 0 up to 1000 are reserved for builtin use
 * custom exception types should therefore have a value > 1000
 * */
typedef enum {
    flux_exception_type_division_by_zero = 0,
} FluxExceptionType;

typedef struct {
    unsigned int from_instruction;
    unsigned int to_instruction;
    unsigned int jump_instruction;

    FluxExceptionType exception_type;
} FluxException;


FluxException* flux_exception_init(unsigned int from, unsigned int to, unsigned int jump, FluxExceptionType type);
void flux_exception_free(FluxException*);

#endif
