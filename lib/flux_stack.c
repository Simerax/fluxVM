#include"flux_stack.h"
#include"flux_log.h"
#include<stdlib.h>

flux_stack* flux_stack_init() {
    flux_stack* stack = malloc(sizeof(flux_stack));
    FLUX_DLOG("Initializing flux_stack %p", stack);
    stack->index = 0;
    stack->objects = malloc(sizeof(flux_object*[FLUX_STACK_SIZE]));
    return stack;
}

void flux_stack_free(flux_stack* stack) {
    if(stack == NULL)
        return;

    FLUX_DLOG("Freeing flux_stack %p", stack);

    for(int i = 0; i < FLUX_STACK_SIZE; i++) 
        flux_object_free(stack->objects[i]);

    free(stack);
}

void flux_stack_pop(flux_stack* stack) {
    if(stack->index == 0) {
        FLUX_DLOG("Trying to pop a value from an empty stack!", "");
        return; 
    }
    stack->index--;
    flux_object_dec_ref(stack->objects[stack->index]);
    stack->objects[stack->index] = NULL; // make sure we don't do a double free on cleanup
}

void flux_stack_ipush(flux_stack* stack, int value) {
    flux_object* integer = flux_object_iinit(value);
    stack->objects[stack->index] = integer;
    stack->index++;
}

void flux_stack_iadd(flux_stack* stack) {
    flux_object* a = flux_stack_get_noffset(stack, 1);
    flux_object* b = flux_stack_get_noffset(stack, 2);
    int result = *((int*)a->value) + *((int*)b->value);
    flux_stack_pop(stack);
    flux_stack_pop(stack);
    flux_stack_ipush(stack, result);
}


flux_object* flux_stack_get_noffset(flux_stack* stack, unsigned int offset) {
    return stack->objects[stack->index - offset];
}

void flux_stack_print(flux_stack* stack) {
    flux_object* obj = flux_stack_get_noffset(stack, 1);
    flux_object_print(obj);
    flux_stack_pop(stack);
}

