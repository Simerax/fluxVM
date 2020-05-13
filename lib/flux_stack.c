#include"flux_stack.h"
#include"flux_log.h"
#include<stdlib.h>

FluxStack* flux_stack_init() {
    FluxStack* stack = malloc(sizeof(FluxStack));
    FLUX_DLOG("Initializing FluxStack %p", stack);
    stack->index = 0;
    stack->error = flux_stack_error_no_error;
    stack->objects = malloc(sizeof(FluxObject*[FLUX_STACK_SIZE]));
    return stack;
}

void flux_stack_free(FluxStack* stack) {
    if(stack == NULL)
        return;

    FLUX_DLOG("Freeing FluxStack %p", stack);

    for(int i = 0; i < FLUX_STACK_SIZE; i++) {
        if(stack->objects[i] != NULL) {
            FLUX_WLOG("Non Empty Stack on Cleanup!");
            flux_object_dec_ref(stack->objects[i]);
        }
    }

    free(stack);
}

void flux_stack_pop(FluxStack* stack) {
    if(stack->index == 0) {
        FLUX_ELOG("Trying to pop a value from an empty stack!", "");
        flux_stack_set_error(stack, flux_stack_error_underflow);
        return; 
    }
    stack->index--;
    flux_object_dec_ref(stack->objects[stack->index]);
    stack->objects[stack->index] = NULL; // make sure we don't do a double free on cleanup
}

void flux_stack_ipush(FluxStack* stack, int value) {
    if(stack->index == FLUX_STACK_SIZE - 1) {
        FLUX_ELOG("Tried pushing onto full stack %p", stack);
        flux_stack_set_error(stack, flux_stack_error_overflow);
        return;
    }
    FluxObject* integer = flux_object_iinit(value);
    stack->objects[stack->index] = integer;
    stack->index++;
}

void flux_stack_push(FluxStack* stack, FluxObject* obj) {
    if(stack->index == FLUX_STACK_SIZE -1) {
        FLUX_ELOG("Tried to push object on full stack %p", stack);
        flux_stack_set_error(stack, flux_stack_error_overflow);
        return;
    }
    flux_object_inc_ref(obj);
    stack->objects[stack->index] = obj;
    stack->index++;
}

void flux_stack_iadd(FluxStack* stack) {
    FluxObject* a = flux_stack_get_noffset(stack, 1);
    FluxObject* b = flux_stack_get_noffset(stack, 2);
    if(a != NULL && b != NULL) {
        int result = *((int*)a->value) + *((int*)b->value);
        flux_stack_pop(stack);
        flux_stack_pop(stack);
        flux_stack_ipush(stack, result);
    } else {
        FLUX_ELOG("Cannot do integer add on NULL objects a: %p b: %p", a, b);
    }
}


FluxObject* flux_stack_get_noffset(FluxStack* stack, int offset) {
    if (stack->index - offset < 0) {
        flux_stack_set_error(stack, flux_stack_error_underflow_access);
        return NULL;
    }

    return stack->objects[stack->index - offset];
}

void flux_stack_print(FluxStack* stack) {
    FluxObject* obj = flux_stack_get_noffset(stack, 1);

    if(obj != NULL) {
        flux_object_print(obj);
        flux_stack_pop(stack);
    } else {
        FLUX_ELOG("Tried to print Stack Object but Object is NULL!");
    }
}

void flux_stack_set_error(FluxStack* stack, FluxStackError err) {
    FLUX_ELOG("flux_stack_set_error: %d", err);
    stack->error = err;
}

FluxStackError flux_stack_get_error(FluxStack* stack) {
    return stack->error;
}

void flux_stack_reset_error(FluxStack* stack) {
    stack->error = flux_stack_error_no_error;
}

