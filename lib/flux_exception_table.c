#include "flux_exception_table.h"
#include "flux_exception.h"
#include "flux_log.h"

#include<stdlib.h> // malloc

FluxExceptionTable* flux_exception_table_init() {
    FluxExceptionTable* ext = malloc(sizeof(FluxExceptionTable));
    FLUX_DLOG("Initializing FluxExceptionTable %p", ext);
    ext->number_of_exceptions = 0;
    ext->table_capacity = 10;
    ext->exceptions = malloc(sizeof(FluxException) * ext->table_capacity);

    for(int i = 0; i < ext->table_capacity; i++)
        ext->exceptions[i] = NULL;

    return ext;
}

void flux_exception_table_free(FluxExceptionTable* ext) {
    if(ext == NULL)
        return;

    FLUX_DLOG("Freeing FluxExceptionTable %p", ext);

    for(int i = 0; i < ext->table_capacity; i++) {
        if(ext->exceptions[i] != NULL) {
            free(ext->exceptions[i]);
        }
    }
    free(ext->exceptions);
    free(ext);
}


void flux_exception_table_add(FluxExceptionTable* ext, FluxException* ex) {
    if (ext->number_of_exceptions >= ext->table_capacity) {
        ext->table_capacity += 10;
        ext->exceptions = realloc(ext->exceptions, ext->table_capacity);
    }

    ext->exceptions[ext->number_of_exceptions] = ex;
    ext->number_of_exceptions++;
}

FluxException* flux_exception_table_lookup(FluxExceptionTable* table, FluxExceptionType type, unsigned int at_instruction) {
    for(int i = 0; i < table->number_of_exceptions; i++) {
        FluxException* ex = table->exceptions[i];
        if(ex->exception_type == type && ex->from_instruction <= at_instruction && ex->to_instruction >= at_instruction) {
            return ex;
        }
    }
    return NULL;
}

