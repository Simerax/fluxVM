#ifndef FLUX_CODE_H
#define FLUX_CODE_H

#include "flux_command.h"
#include "flux_exception_table.h"

typedef struct {
    FluxCommand** commands;
    int number_of_commands;
    FluxExceptionTable* exception_table;
} FluxCode;



FluxCode* flux_code_init(char* bytes, int length);
void flux_code_free(FluxCode*);
size_t flux_code_length(FluxCode*);
char* flux_code_code_ptr(FluxCode*);
int flux_code_convert_to_flux_commands(char* bytes, int length, FluxCommand*** commands, FluxExceptionTable** table);

int read_integer(char*);

#endif //FLUX_CODE_H
