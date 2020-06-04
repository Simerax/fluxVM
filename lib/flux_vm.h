#ifndef FLUX_VM_H
#define FLUX_VM_H

#include "flux_exception.h"
#include "flux_exception_table.h"
#include"flux_stack.h"
#include "flux_code.h"
#include "flux_cmp_result.h"
#include "flux_error.h"

#define FLUX_MAX_VARS 256

typedef enum {
    flux_vm_error_type_no_error = 0,
    flux_vm_error_type_uncaught_exception = 1,
} FluxVMErrorType;

typedef struct {
    FluxVMErrorType type;
    unsigned int position; // instruction index
} FluxVMError;

FluxVMError flux_vm_error_init();
FluxVMError flux_vm_error_init_no_error();
FluxVMError flux_vm_error_init_uncaught_exception();


typedef struct {
    FluxStack* stack;
    FluxObject** vars;
    int instruction_index;
    FluxCmpResult cmp_flag;
    bool did_jump;
    FluxCode* code;
    bool uncaught_exception_flag;
    bool exit_flag;
} FluxVM;


FluxVM* flux_vm_init();
void flux_vm_free(FluxVM*);

void flux_vm_print(FluxVM*);
void flux_vm_inspect(FluxVM*);

void flux_vm_store(FluxVM*);
void flux_vm_store_index(FluxVM*, unsigned int);
void flux_vm_load(FluxVM*);
void flux_vm_load_index(FluxVM*, unsigned int);

void flux_vm_ipush(FluxVM*, int value);
void flux_vm_push(FluxVM*, FluxObject*);
void flux_vm_iadd(FluxVM*);
void flux_vm_isub(FluxVM*);
void flux_vm_imul(FluxVM*);
FluxArithmeticError flux_vm_idiv(FluxVM*);
void flux_vm_pop(FluxVM*);

void flux_vm_throw_internal(FluxVM*, FluxExceptionType, FluxExceptionTable*);
void flux_vm_throw(FluxVM*);

void flux_vm_itod(FluxVM*);

FluxVMError flux_vm_execute(FluxVM*, FluxCode*);

void flux_vm_jmp(FluxVM*, FluxObject*);
bool flux_vm_je(FluxVM*, FluxObject*);
bool flux_vm_jl(FluxVM*, FluxObject*);
bool flux_vm_jle(FluxVM*, FluxObject*);
bool flux_vm_jge(FluxVM*, FluxObject*);
void flux_vm_jsr(FluxVM*, FluxObject*);

void flux_vm_cmp(FluxVM*);

void flux_vm_ret(FluxVM*);

void flux_vm_inc_ref(FluxVM*, FluxObject*);
void flux_vm_dec_ref(FluxVM*, FluxObject*);



#endif //FLUX_VM_H
