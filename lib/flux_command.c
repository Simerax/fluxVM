#include"flux_command.h"
#include<stdlib.h>


flux_command* flux_command_init(flux_instruction fi, flux_object** parameters, int number_of_parameters) {
    flux_command* cmd = malloc(sizeof(flux_command));
    cmd->instruction = fi;
    cmd->parameters = parameters;
    cmd->number_of_parameters = number_of_parameters;
    return cmd;
}

void flux_command_free(flux_command* cmd) {
    if(cmd == NULL)
        return;
    
    for(int i = 0; i < cmd->number_of_parameters; i++) {
        if(cmd->parameters[i] != NULL)
            flux_object_dec_ref(cmd->parameters[i]);
    }
    free(cmd);
}