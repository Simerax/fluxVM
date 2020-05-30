#include"flux_exception.h"
#include "flux_log.h"

#include<stdlib.h> // malloc
#include<string.h> // memcpy

FluxException* flux_exception_init(unsigned int from, unsigned int to, unsigned int jump, char* type, unsigned int type_str_length) {
    FluxException* obj = malloc(sizeof(FluxException));
    FLUX_DLOG("Initializing Flux Exception %p", obj);
    obj->from_instruction = from;
    obj->to_instruction = to;
    obj->jump_instruction = jump;
    obj->type_name = malloc(type_str_length);
    obj->type_name_length = type_str_length;
    // TODO: Maybe move instead of copy
    memcpy(obj->type_name, type, type_str_length);
    return obj;
}

void flux_exception_free(FluxException* ex) {
    if(ex == NULL)
        return;
    FLUX_DLOG("Freeing FluxException %p", ex);
    free(ex->type_name);
    free(ex);
}
