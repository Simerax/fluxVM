#ifndef FLUX_VM_H
#define FLUX_VM_H

#include"flux_stack.h"
#include"flux_code.h"

#define FLUX_MAX_VARS 256

typedef struct {
    FluxStack* stack;
    flux_object** vars;
} FluxVM;


FluxVM* flux_vm_init();
void flux_vm_free(FluxVM*);

void flux_vm_print(FluxVM*);

void flux_vm_store(FluxVM*);
void flux_vm_load(FluxVM*, unsigned int index);

void flux_vm_ipush(FluxVM*, int value);
void flux_vm_push(FluxVM*, flux_object*);
void flux_vm_iadd(FluxVM*);
void flux_vm_pop(FluxVM*);

void flux_vm_itod(FluxVM*);

void flux_vm_execute(FluxVM*, flux_code*);


#endif //FLUX_VM_H
