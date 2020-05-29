#include"flux_vm.h"
#include "flux_cmp_result.h"
#include"flux_log.h"
#include "flux_object.h"
#include"flux_stack.h"

#include<stdlib.h>


FluxVM* flux_vm_init() {
    FluxVM* vm = malloc(sizeof(FluxVM));
    FLUX_DLOG("Initializing FluxVM %p", vm);

    vm->stack = flux_stack_init();
    vm->vars = malloc(sizeof(FluxObject*[FLUX_MAX_VARS]));
    vm->instruction_index = 0;
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

    FLUX_DLOG("STORING Variable %p at index: %d (index_obj: %p)", value_obj, index, index_obj);

    flux_object_dec_ref(vm->vars[index]);
    vm->vars[index] = value_obj;

    flux_stack_pop(vm->stack);
    flux_stack_pop(vm->stack);
}

void flux_vm_load(FluxVM* vm) {
    FluxObject* index_obj = flux_stack_get_noffset(vm->stack, 1);

    int index = flux_object_get_int_value(index_obj);
    FluxObject* obj = vm->vars[index];
    flux_vm_pop(vm); // get rid of the index_obj
    flux_stack_push(vm->stack, obj);
}
void flux_vm_ipush(FluxVM* vm, int value) {
    flux_stack_ipush(vm->stack, value);
}
void flux_vm_iadd(FluxVM* vm) {
    flux_stack_iadd(vm->stack);
}

void flux_vm_isub(FluxVM* vm) {
    flux_stack_isub(vm->stack);
}

void flux_vm_imul(FluxVM* vm) {
    flux_stack_imul(vm->stack);
}

void flux_vm_idiv(FluxVM* vm) {
    flux_stack_idiv(vm->stack);
}

void flux_vm_print(FluxVM* vm) {
    flux_stack_print(vm->stack);
}

void flux_vm_inspect(FluxVM* vm) {
    flux_stack_inspect(vm->stack);
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

void flux_vm_jmp(FluxVM* vm, FluxObject* address) {
    vm->instruction_index = flux_object_get_int_value(address);
    FLUX_DLOG("JMP to index %d", vm->instruction_index);
}

bool flux_vm_je(FluxVM* vm, FluxObject* address) {

    if(vm->cmp_flag == EQUAL) {
        vm->instruction_index = flux_object_get_int_value(address);
        FLUX_DLOG("JE to index %d", vm->instruction_index);
        vm->cmp_flag = NONE;
        return true;
    } else {
        return false;
    }
}

bool flux_vm_jl(FluxVM* vm, FluxObject* address) {

    if(vm->cmp_flag == LESS) {
        vm->instruction_index = flux_object_get_int_value(address);
        FLUX_DLOG("JL to index %d", vm->instruction_index);
        vm->cmp_flag = NONE;
        return true;
    } else {
        return false;
    }
}

bool flux_vm_jle(FluxVM* vm, FluxObject* address) {

    if(vm->cmp_flag == LESS || vm->cmp_flag == EQUAL) {
        vm->instruction_index = flux_object_get_int_value(address);
        FLUX_DLOG("JLE to index %d", vm->instruction_index);
        vm->cmp_flag = NONE;
        return true;
    } else {
        return false;
    }
}

bool flux_vm_jge(FluxVM* vm, FluxObject* address) {

    if(vm->cmp_flag == GREATER || vm->cmp_flag == EQUAL) {
        vm->instruction_index = flux_object_get_int_value(address);
        FLUX_DLOG("JLE to index %d", vm->instruction_index);
        vm->cmp_flag = NONE;
        return true;
    } else {
        return false;
    }
}

void flux_vm_execute(FluxVM* vm, FluxCode* code) {

    vm->instruction_index = 0;

    while(vm->instruction_index < code->number_of_commands) {
        FluxCommand* cmd = code->commands[vm->instruction_index];

        bool did_jump = false;


        switch(cmd->instruction) {
            case IPUSH: flux_vm_push(vm, cmd->parameters[0]);
                        break;
            case SPUSH: flux_vm_push(vm, cmd->parameters[0]);
                        break;
            case IADD: flux_vm_iadd(vm);
                       break;
            case ISUB: flux_vm_isub(vm);
                       break;
            case IMUL: flux_vm_imul(vm);
                       break;
            case IDIV: flux_vm_idiv(vm);
                       break;
            case ITOD: flux_vm_itod(vm);
                       break;
            case POP: flux_vm_pop(vm);
                      break;
            case LOAD: flux_vm_load(vm);
                       break;
            case STORE: flux_vm_store(vm);
                        break;
            case PRINT: flux_vm_print(vm);
                        break;
            case INSPECT: flux_vm_inspect(vm);
                          break;
            case JSR: flux_vm_ipush(vm, vm->instruction_index);
                      flux_vm_jmp(vm, cmd->parameters[0]);
                      did_jump = true;
                      break;
            case CMP: flux_vm_cmp(vm);
                      break;
            case JE: if(flux_vm_je(vm, cmd->parameters[0]))
                         did_jump = true;
                     break;
            case JL: if(flux_vm_jl(vm, cmd->parameters[0]))
                         did_jump = true;
                     break;
            case JLE: if(flux_vm_jle(vm, cmd->parameters[0]))
                          did_jump = true;
                      break;
            case JGE: if(flux_vm_jge(vm, cmd->parameters[0]))
                          did_jump = true;
                      break;
                     
                     break;
            default: FLUX_ELOG("Unknown Instruction %d", cmd->instruction);
                     break;
        }

        if(did_jump)
            did_jump = false;
        else
            vm->instruction_index++;
    }
}

void flux_vm_cmp(FluxVM* vm) {
    FluxObject* a = flux_stack_get_noffset(vm->stack, 2);
    FluxObject* b = flux_stack_get_noffset(vm->stack, 1);

    vm->cmp_flag = flux_object_cmp(a, b);
    FLUX_DLOG("CMP result %d", vm->cmp_flag);

    flux_stack_pop(vm->stack);
    flux_stack_pop(vm->stack);
}

