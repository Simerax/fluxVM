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
