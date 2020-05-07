#ifndef FLUX_VM_H
#define FLUX_VM_H

#include"flux_stack.h"

#define FLUX_MAX_VARS 256

typedef struct {
    flux_stack* stack;
    flux_object** vars;
} flux_vm;


flux_vm* flux_vm_init();
void flux_vm_free(flux_vm*);

void flux_vm_print(flux_vm*);

void flux_vm_store(flux_vm*);
void flux_vm_load(flux_vm*, unsigned int index);

void flux_vm_ipush(flux_vm*, int value);
void flux_vm_iadd(flux_vm*);
void flux_vm_pop(flux_vm*);


#endif //FLUX_VM_H
