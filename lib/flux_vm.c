#include"flux_vm.h"
#include "flux_cmp_result.h"
#include "flux_error.h"
#include "flux_exception.h"
#include "flux_exception_table.h"
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
    vm->did_jump = false;
    vm->code = NULL;
    vm->cmp_flag = NONE;
    vm->uncaught_exception_flag = false;
    vm->exit_flag = false;
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
    free(vm->vars);
    free(vm);
}

void flux_vm_store_index(FluxVM* vm, unsigned int index) {
    FluxObject* obj = flux_stack_get_noffset(vm->stack, 1);

    if(obj == NULL) {
        FLUX_ELOG("Tried storing a NULL object");
        return;
    }

    flux_object_inc_ref(obj);
    flux_object_dec_ref(vm->vars[index]);
    vm->vars[index] = obj;
    flux_stack_pop(vm->stack);
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


void flux_vm_load_index(FluxVM* vm, unsigned int index) {
    FluxObject* obj = vm->vars[index];
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

FluxArithmeticError flux_vm_idiv(FluxVM* vm) {
    return flux_stack_idiv(vm->stack);
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
    vm->did_jump = true;
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

FluxVMError flux_vm_execute(FluxVM* vm, FluxCode* code) {

    vm->code = code;
    vm->instruction_index = 0;
    vm->did_jump = false;

    while(vm->instruction_index < code->number_of_commands) {
        FluxCommand* cmd = code->commands[vm->instruction_index];

        FluxArithmeticError error = no_error;


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
            case IDIV: 
                       error = flux_vm_idiv(vm);
                       if(error == division_by_zero) {
                           flux_vm_throw_internal(vm, flux_exception_type_division_by_zero, code->exception_table);
                           error = no_error;
                       }
                       break;
            case ITOD: flux_vm_itod(vm);
                       break;
            case POP: flux_vm_pop(vm);
                      break;
            case LOAD: flux_vm_load(vm);
                       break;
            case LOAD0: flux_vm_load_index(vm, 0);
                        break;
            case LOAD1: flux_vm_load_index(vm, 1);
                        break;
            case LOAD2: flux_vm_load_index(vm, 2);
                        break;
            case LOAD3: flux_vm_load_index(vm, 3);
                        break;
            case STORE: flux_vm_store(vm);
                        break;
            case STORE0: flux_vm_store_index(vm, 0);
                         break;
            case STORE1: flux_vm_store_index(vm, 1);
                         break;
            case STORE2: flux_vm_store_index(vm, 2);
                         break;
            case STORE3: flux_vm_store_index(vm, 3);
                         break;
            case PRINT: flux_vm_print(vm);
                        break;
            case INSPECT: flux_vm_inspect(vm);
                          break;
            case JSR: flux_vm_jsr(vm, cmd->parameters[0]);
                      break;
            case CMP: flux_vm_cmp(vm);
                      break;
            case JE: if(flux_vm_je(vm, cmd->parameters[0]))
                         vm->did_jump = true;
                     break;
            case JL: if(flux_vm_jl(vm, cmd->parameters[0]))
                         vm->did_jump = true;
                     break;
            case JLE: if(flux_vm_jle(vm, cmd->parameters[0]))
                          vm->did_jump = true;
                      break;
            case JGE: if(flux_vm_jge(vm, cmd->parameters[0]))
                          vm->did_jump = true;
                      break;
            case THROW: flux_vm_throw(vm);
                        break;
            case RET: flux_vm_ret(vm);
                      break;
            case EXIT: vm->exit_flag = true;
                       break;
            default: FLUX_ELOG("Unknown Instruction %d", cmd->instruction);
                     break;
        }

        if(vm->exit_flag == true) {
            break;
        }

        if(vm->uncaught_exception_flag == true) {
            FluxVMError err;
            err.type = flux_vm_error_type_uncaught_exception;
            err.position = vm->instruction_index;
            return err;
        }

        if(vm->did_jump == true)
            vm->did_jump = false;
        else
            vm->instruction_index++;
    }

    FluxVMError no_error;
    no_error.type = flux_vm_error_type_no_error;
    no_error.position = 0;
    return no_error;
}

void flux_vm_throw_internal(FluxVM *vm, FluxExceptionType type, FluxExceptionTable *table) {
    FluxException* ex = flux_exception_table_lookup(table, type, vm->instruction_index);
    if(ex != NULL) {
        vm->instruction_index = ex->jump_instruction;
        vm->did_jump = true;
    } else {
        vm->uncaught_exception_flag = true;
        FLUX_ELOG("Uncaught Exception at Instruction %d", vm->instruction_index);
    }
}

void flux_vm_throw(FluxVM* vm) {
    FluxObject* object_to_throw = flux_stack_get_noffset(vm->stack, 1);


    FluxExceptionType type = flux_object_get_exception_type(object_to_throw);
    FluxException* ex = flux_exception_table_lookup(vm->code->exception_table, type, vm->instruction_index);
    if(ex == NULL) {
        vm->uncaught_exception_flag = true;
        FLUX_ELOG("Uncaught Exception at Instruction %d", vm->instruction_index);
    } else {
        vm->instruction_index = ex->jump_instruction;
        vm->did_jump = true;
        flux_object_inc_ref(object_to_throw); // make sure the variable does not get freed when the stack is cleared
        flux_stack_clear(vm->stack);
        flux_stack_push(vm->stack, object_to_throw);
        flux_object_dec_ref(object_to_throw);
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

void flux_vm_jsr(FluxVM* vm, FluxObject* address) {
    FluxObject* frame = flux_object_frame_init(vm->instruction_index + 1);
    flux_vm_push(vm, frame);
    flux_vm_jmp(vm, address);
}

void flux_vm_ret(FluxVM* vm) {
    FluxObject* return_value = flux_stack_get_noffset(vm->stack, 1);
    flux_object_inc_ref(return_value);
    flux_stack_clean_frame(vm->stack);
    FluxObject* frame = flux_stack_get_noffset(vm->stack, 1);
    flux_vm_jmp(vm, frame);
    flux_stack_pop(vm->stack); // get rid of the Frame Object
    flux_stack_push(vm->stack, return_value);
    flux_object_dec_ref(return_value);
}

