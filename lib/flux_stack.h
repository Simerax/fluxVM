#ifndef FLUX_STACK_H
#define FLUX_STACK_H
#include"flux_object.h"

#define FLUX_STACK_SIZE 65536

typedef enum {
    flux_stack_error_no_error, // No Error
    flux_stack_error_overflow, // Stack Index is greater STACK_SIZE
    flux_stack_error_underflow, // Stack Index is smaller 0
    flux_stack_error_underflow_access, // Tried to access a Stack Element at index smaller 0
} FluxStackError;

typedef struct {
    int index; // not unsigned because we might need to detect an underflow
    FluxStackError error;
    FluxObject** objects;
} FluxStack;


FluxStack* flux_stack_init();
void flux_stack_free(FluxStack*);

void flux_stack_pop(FluxStack*);
void flux_stack_push(FluxStack*, FluxObject*);
void flux_stack_ipush(FluxStack*, int value);
void flux_stack_cpush(FluxStack*, char value);
void flux_stack_iadd(FluxStack*);
void flux_stack_isub(FluxStack*);
void flux_stack_imul(FluxStack*);

void flux_stack_set_error(FluxStack*, FluxStackError);
FluxStackError flux_stack_get_error(FluxStack*);
void flux_stack_reset_error(FluxStack*);


// get stack object at negative offset
FluxObject* flux_stack_get_noffset(FluxStack*, int offset);

void flux_stack_print(FluxStack*);
void flux_stack_inspect(FluxStack*);




#endif //FLUX_STACK_H
