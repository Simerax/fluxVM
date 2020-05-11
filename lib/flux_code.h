#ifndef FLUX_CODE_H
#define FLUX_CODE_H

#include"flux_command.h"

typedef struct {
    flux_command** commands;
    int number_of_commands;
} flux_code;



flux_code* flux_code_init(char* bytes, int length);
size_t flux_code_length(flux_code*);
char* flux_code_code_ptr(flux_code*);
int flux_code_convert_to_flux_commands(char* bytes, int length, flux_command*** commands);

#endif //FLUX_CODE_H
