#ifndef FLUX_STACK_H
#define FLUX_STACK_H
#include"flux_object.h"

#define FLUX_STACK_SIZE 65536

typedef enum {
    flux_stack_error_no_error, // No Error
    flux_stack_error_overflow, // Stack Index is greater STACK_SIZE
    flux_stack_error_underflow, // Stack Index is smaller 0
    flux_stack_error_underflow_access, // Tried to access a Stack Element at index smaller 0
} flux_stack_error;

typedef struct {
    int index; // not unsigned because we might need to detect an underflow
    flux_stack_error error;
    flux_object** objects;
} FluxStack;


FluxStack* flux_stack_init();
void flux_stack_free(FluxStack*);

void flux_stack_pop(FluxStack*);
void flux_stack_push(FluxStack*, flux_object*);
void flux_stack_ipush(FluxStack*, int value);
void flux_stack_iadd(FluxStack*);

void flux_stack_set_error(FluxStack*, flux_stack_error);
flux_stack_error flux_stack_get_error(FluxStack*);
void flux_stack_reset_error(FluxStack*);


// get stack object at negative offset
flux_object* flux_stack_get_noffset(FluxStack*, int offset);

void flux_stack_print(FluxStack*);




#endif //FLUX_STACK_H
