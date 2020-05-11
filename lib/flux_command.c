#include"flux_command.h"
#include<stdlib.h>


flux_command* flux_command_init(flux_instruction fi, flux_object** parameters, int number_of_parameters) {
    flux_command* cmd = malloc(sizeof(flux_command));
    cmd->instruction = fi;
    cmd->parameters = parameters;
    cmd->number_of_parameters = number_of_parameters;
    return cmd;
}
