#include"flux_exception.h"
#include "flux_log.h"

#include<stdlib.h> // malloc
#include<string.h> // memcpy

FluxException* flux_exception_init(unsigned int from, unsigned int to, unsigned int jump, FluxExceptionType type) {
    FluxException* obj = malloc(sizeof(FluxException));
    FLUX_DLOG("Initializing Flux Exception %p", obj);
    obj->from_instruction = from;
    obj->to_instruction = to;
    obj->jump_instruction = jump;
    obj->exception_type = type;
    return obj;
}

void flux_exception_free(FluxException* ex) {
    if(ex == NULL)
        return;
    FLUX_DLOG("Freeing FluxException %p", ex);
    free(ex);
}
