#include"flux_stack.h"
#include"flux_log.h"
#include<stdlib.h>

flux_stack* flux_stack_init() {
    flux_stack* stack = malloc(sizeof(flux_stack));
    FLUX_DLOG("Initializing flux_stack %p", stack);
    stack->index = 0;
    stack->error = flux_stack_error_no_error;
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
        FLUX_ELOG("Trying to pop a value from an empty stack!", "");
        flux_stack_set_error(stack, flux_stack_error_underflow);
        return; 
    }
    stack->index--;
    flux_object_dec_ref(stack->objects[stack->index]);
    stack->objects[stack->index] = NULL; // make sure we don't do a double free on cleanup
}

void flux_stack_ipush(flux_stack* stack, int value) {
    if(stack->index == FLUX_STACK_SIZE - 1) {
        FLUX_ELOG("Tried pushing onto full stack %p", stack);
        flux_stack_set_error(stack, flux_stack_error_overflow);
        return;
    }
    flux_object* integer = flux_object_iinit(value);
    stack->objects[stack->index] = integer;
    stack->index++;
}

void flux_stack_iadd(flux_stack* stack) {
    flux_object* a = flux_stack_get_noffset(stack, 1);
    flux_object* b = flux_stack_get_noffset(stack, 2);
    if(a != NULL && b != NULL) {
        int result = *((int*)a->value) + *((int*)b->value);
        flux_stack_pop(stack);
        flux_stack_pop(stack);
        flux_stack_ipush(stack, result);
    } else {
        FLUX_ELOG("Cannot do integer add on NULL objects a: %p b: %p", a, b);
    }
}


flux_object* flux_stack_get_noffset(flux_stack* stack, int offset) {
    if (stack->index - offset < 0) {
        flux_stack_set_error(stack, flux_stack_error_underflow_access);
        return NULL;
    }

    return stack->objects[stack->index - offset];
}

void flux_stack_print(flux_stack* stack) {
    flux_object* obj = flux_stack_get_noffset(stack, 1);

    if(obj != NULL) {
        flux_object_print(obj);
        flux_stack_pop(stack);
    } else {
        FLUX_ELOG("Tried to print Stack Object but Object is NULL!");
    }
}

void flux_stack_set_error(flux_stack* stack, flux_stack_error err) {
    FLUX_ELOG("flux_stack_set_error: %d", err);
    stack->error = err;
}

flux_stack_error flux_stack_get_error(flux_stack* stack) {
    return stack->error;
}

void flux_stack_reset_error(flux_stack* stack) {
    stack->error = flux_stack_error_no_error;
}

