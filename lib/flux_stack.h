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
} flux_stack;


flux_stack* flux_stack_init();
void flux_stack_free(flux_stack*);

void flux_stack_pop(flux_stack*);
void flux_stack_ipush(flux_stack*, int value);
void flux_stack_iadd(flux_stack*);

void flux_stack_set_error(flux_stack*, flux_stack_error);
flux_stack_error flux_stack_get_error(flux_stack*);
void flux_stack_reset_error(flux_stack*);


// get stack object at negative offset
flux_object* flux_stack_get_noffset(flux_stack*, int offset);

void flux_stack_print(flux_stack*);




#endif //FLUX_STACK_H
