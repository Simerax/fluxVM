#ifndef FLUX_COMMAND_H
#define FLUX_COMMAND_H
#include "flux_instruction.h"
#include "flux_object.h"

typedef struct {
    flux_instruction instruction;
    flux_object** parameters;
    int number_of_parameters;
} flux_command;


flux_command* flux_command_init(flux_instruction, flux_object** parameters, int number_of_parameters);


#endif //FLUX_COMMAND_H
