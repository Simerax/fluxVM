#include"flux_vm.h"
#include"flux_log.h"
#include"flux_stack.h"

#include<stdlib.h>


flux_vm* flux_vm_init() {
    flux_vm* vm = malloc(sizeof(flux_vm));
    FLUX_DLOG("Initializing flux_vm %p", vm);

    vm->stack = flux_stack_init();
    vm->vars = malloc(sizeof(flux_object*[FLUX_MAX_VARS]));

    return vm;
}

void flux_vm_free(flux_vm* vm) {
    if(vm == NULL)
        return;

    flux_stack_free(vm->stack);

    for(int i = 0; i < FLUX_MAX_VARS; i++)
        flux_object_dec_ref(vm->vars[i]);

    FLUX_DLOG("Freeing flux_vm %p", vm);
    free(vm);
}


void flux_vm_store(flux_vm* vm) {
    flux_object* index_obj = flux_stack_get_noffset(vm->stack, 1);
    flux_object* value_obj = flux_stack_get_noffset(vm->stack, 2);

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

void flux_vm_load(flux_vm* vm, unsigned int index) {
    flux_object* obj = vm->vars[index];
    flux_stack_push(vm->stack, obj);
}
void flux_vm_ipush(flux_vm* vm, int value) {
    flux_stack_ipush(vm->stack, value);
}
void flux_vm_iadd(flux_vm* vm) {
    flux_stack_iadd(vm->stack);
}
void flux_vm_print(flux_vm* vm) {
    flux_stack_print(vm->stack);
}

void flux_vm_pop(flux_vm* vm) {
    flux_stack_pop(vm->stack);
}

void flux_vm_push(flux_vm* vm, flux_object* obj) {
    flux_stack_push(vm->stack, obj);
}

void flux_vm_itod(flux_vm* vm) {
    flux_object* integer_obj = flux_stack_get_noffset(vm->stack, 1);
    if(integer_obj == NULL) {
        FLUX_ELOG("Tried converting integer to double but stack is empty");
        return;
    }

    if(integer_obj->type == Double)
        return;

    if(integer_obj->type == Integer) {
        flux_object* copy = flux_object_copy(integer_obj);
        flux_stack_pop(vm->stack);
        flux_object_itod(copy);
        flux_stack_push(vm->stack, copy);
    } else {
        FLUX_ELOG("Tried converting int to double but int was of type %d  obj: %p", integer_obj->type, integer_obj);
    }
}

void flux_vm_execute(flux_vm* vm, flux_code* code) {

    int current_command_index = 0;

    while(current_command_index < code->number_of_commands) {
        flux_command* cmd = code->commands[current_command_index];
        current_command_index++;


        switch(cmd->instruction) {
            case IPUSH: flux_vm_push(vm, cmd->parameters[0]);
                        break;
            case IADD: flux_vm_iadd(vm);
                       break;
            default: FLUX_ELOG("Unknown Instruction %d", cmd->instruction);
                     break;
        }
    }
}

