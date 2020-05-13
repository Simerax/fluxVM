#ifndef FLUX_COMMAND_H
#define FLUX_COMMAND_H
#include "flux_instruction.h"
#include "flux_object.h"

typedef struct {
    FluxInstruction instruction;
    FluxObject** parameters;
    int number_of_parameters;
} FluxCommand;


FluxCommand* flux_command_init(FluxInstruction, FluxObject** parameters, int number_of_parameters);
void flux_command_free(FluxCommand*);


#endif //FLUX_COMMAND_H
