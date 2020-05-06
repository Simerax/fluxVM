#include"flux_vm.h"
#include"flux_log.h"
#include"flux_stack.h"

#include<stdlib.h>


flux_vm* flux_vm_init() {
    flux_vm* vm = malloc(sizeof(flux_vm));
    FLUX_DLOG("Initializing flux_vm %p", vm);

    vm->stack = flux_stack_init();

    return vm;
}

void flux_vm_free(flux_vm* vm) {
    if(vm == NULL)
        return;

    flux_stack_free(vm->stack);
    FLUX_DLOG("Freeing flux_vm %p", vm);
    free(vm);
}


void flux_vm_istore(flux_vm* vm, unsigned int index, int value) {
}
void flux_vm_iload(flux_vm* vm, unsigned int index) {
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
