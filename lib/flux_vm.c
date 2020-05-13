#include"flux_vm.h"
#include"flux_log.h"
#include"flux_stack.h"

#include<stdlib.h>


FluxVM* flux_vm_init() {
    FluxVM* vm = malloc(sizeof(FluxVM));
    FLUX_DLOG("Initializing FluxVM %p", vm);

    vm->stack = flux_stack_init();
    vm->vars = malloc(sizeof(FluxObject*[FLUX_MAX_VARS]));
    for(int i = 0; i < FLUX_MAX_VARS; i++)
        vm->vars[i] = NULL;

    return vm;
}

void flux_vm_free(FluxVM* vm) {
    if(vm == NULL)
        return;

    flux_stack_free(vm->stack);

    for(int i = 0; i < FLUX_MAX_VARS; i++)
        if(vm->vars[i] != NULL)
            flux_object_dec_ref(vm->vars[i]);

    FLUX_DLOG("Freeing FluxVM %p", vm);
    free(vm);
}


void flux_vm_store(FluxVM* vm) {
    FluxObject* index_obj = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* value_obj = flux_stack_get_noffset(vm->stack, 2);

    if(index_obj == NULL || value_obj == NULL) {
        FLUX_ELOG("Tried storing an object with invalid stack: index_obj %p - value_obj: %p", index_obj, value_obj);
        return;
    }

    flux_object_inc_ref(value_obj);

    int index = *((int*)index_obj->value);

    flux_object_dec_ref(vm->vars[index]);
    vm->vars[index] = value_obj;

    flux_stack_pop(vm->stack);
    flux_stack_pop(vm->stack);
}

void flux_vm_load(FluxVM* vm, unsigned int index) {
    FluxObject* obj = vm->vars[index];
    flux_stack_push(vm->stack, obj);
}
void flux_vm_ipush(FluxVM* vm, int value) {
    flux_stack_ipush(vm->stack, value);
}
void flux_vm_iadd(FluxVM* vm) {
    flux_stack_iadd(vm->stack);
}
void flux_vm_print(FluxVM* vm) {
    flux_stack_print(vm->stack);
}

void flux_vm_pop(FluxVM* vm) {
    flux_stack_pop(vm->stack);
}

void flux_vm_push(FluxVM* vm, FluxObject* obj) {
    flux_stack_push(vm->stack, obj);
}

void flux_vm_itod(FluxVM* vm) {
    FluxObject* integer_obj = flux_stack_get_noffset(vm->stack, 1);
    if(integer_obj == NULL) {
        FLUX_ELOG("Tried converting integer to double but stack is empty");
        return;
    }

    if(integer_obj->type == Double)
        return;

    if(integer_obj->type == Integer) {
        FluxObject* copy = flux_object_copy(integer_obj);
        flux_stack_pop(vm->stack);
        flux_object_itod(copy);
        flux_stack_push(vm->stack, copy);
    } else {
        FLUX_ELOG("Tried converting int to double but int was of type %d  obj: %p", integer_obj->type, integer_obj);
    }
}

void flux_vm_execute(FluxVM* vm, FluxCode* code) {

    int current_command_index = 0;

    while(current_command_index < code->number_of_commands) {
        FluxCommand* cmd = code->commands[current_command_index];
        current_command_index++;


        switch(cmd->instruction) {
            case IPUSH: flux_vm_push(vm, cmd->parameters[0]);
                        break;
            case IADD: flux_vm_iadd(vm);
                       break;
            case ITOD: flux_vm_itod(vm);
                       break;
            case POP: flux_vm_pop(vm);
                      break;
            case LOAD: flux_vm_load(vm, flux_object_get_int_value(cmd->parameters[0]));
                       break;
            case STORE: flux_vm_store(vm);
            case PRINT: flux_vm_print(vm);
                        break;
            default: FLUX_ELOG("Unknown Instruction %d", cmd->instruction);
                     break;
        }
    }
}

