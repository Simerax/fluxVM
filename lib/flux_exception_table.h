#ifndef FLUX_EXCEPTION_TABLE_H
#define FLUX_EXCEPTION_TABLE_H

#include "flux_exception.h"
#include "flux_object.h"

typedef struct {
    unsigned int number_of_exceptions;
    unsigned int table_capacity;
    FluxException** exceptions;
} FluxExceptionTable;


FluxExceptionTable* flux_exception_table_init();
void flux_exception_table_free(FluxExceptionTable*);

void flux_exception_table_add(FluxExceptionTable*, FluxException*);
FluxException* flux_exception_table_lookup(FluxExceptionTable* table, char name[], unsigned int at_instruction);


#endif
