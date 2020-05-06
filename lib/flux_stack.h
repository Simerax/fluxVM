#ifndef FLUX_STACK_H
#define FLUX_STACK_H
#include"flux_object.h"

#define FLUX_STACK_SIZE 65536


typedef struct {
    unsigned int index;
    flux_object** objects;
} flux_stack;


flux_stack* flux_stack_init();
void flux_stack_free(flux_stack*);

void flux_stack_pop(flux_stack*);
void flux_stack_ipush(flux_stack*, int value);
void flux_stack_iadd(flux_stack*);


// get stack object at negative offset
flux_object* flux_stack_get_noffset(flux_stack*, unsigned int offset);

void flux_stack_print(flux_stack*);




#endif //FLUX_STACK_H
