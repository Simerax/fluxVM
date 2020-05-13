#include"flux_command.h"
#include<stdlib.h>


FluxCommand* flux_command_init(FluxInstruction fi, FluxObject** parameters, int number_of_parameters) {
    FluxCommand* cmd = malloc(sizeof(FluxCommand));
    cmd->instruction = fi;
    cmd->parameters = parameters;
    cmd->number_of_parameters = number_of_parameters;
    return cmd;
}

void flux_command_free(FluxCommand* cmd) {
    if(cmd == NULL)
        return;
    
    for(int i = 0; i < cmd->number_of_parameters; i++) {
        if(cmd->parameters[i] != NULL)
            flux_object_dec_ref(cmd->parameters[i]);
    }
    free(cmd);
}