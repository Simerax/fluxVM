#include"check.h"
#include "flux_instruction.h"
#include "flux_object.h"
#include "flux_stack.h"
#include"test_helper.h"
#include"flux_vm.h"
#include"flux_log.h"

START_TEST (test_stack_integer_addition)
{
    FluxVM* vm = flux_vm_init();

    flux_vm_ipush(vm, 5);
    flux_vm_ipush(vm, 23);

    flux_vm_iadd(vm);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 28);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_stack_integer_subtraction)
{
    FluxVM* vm = flux_vm_init();

    flux_vm_ipush(vm, 7);
    flux_vm_ipush(vm, 5);

    flux_vm_isub(vm);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 2);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_stack_integer_multiplication)
{
    FluxVM* vm = flux_vm_init();

    flux_vm_ipush(vm, 9);
    flux_vm_ipush(vm, 4);

    flux_vm_imul(vm);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 36);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_stack_integer_division)
{
    FluxVM* vm = flux_vm_init();

    flux_vm_ipush(vm, 10);
    flux_vm_ipush(vm, 5);

    flux_vm_idiv(vm);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 2);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_stack_integer_division_with_remainder)
{
    FluxVM* vm = flux_vm_init();

    flux_vm_ipush(vm, 10);
    flux_vm_ipush(vm, 6);

    flux_vm_idiv(vm);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 1);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_adding_int_variables)
{
    FluxVM* vm = flux_vm_init();

    flux_vm_ipush(vm, 354);
    flux_vm_ipush(vm, 0);
    flux_vm_store(vm);
    flux_vm_ipush(vm, 0);
    flux_vm_load(vm);

    flux_vm_ipush(vm, 6);

    flux_vm_iadd(vm);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 360);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_running_bytecode)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[11] = {
        IPUSH,

        // Integer 20 in big endian notation 4 byte
        0,
        0,
        0,
        20,


        IPUSH,

        // Integer 22 in big endian notation 4 byte
        0,
        0,
        0,
        22,

        IADD,
    };
    // after this bytecode the result of 20 + 22 should be on top of the stack

    FluxCode* code = flux_code_init(bytecode, 11);

    ck_assert_int_eq(code->number_of_commands, 3);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, IADD);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_int_value(result), 42);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_ITOD)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,

        // Integer 20 in big endian notation 4 byte
        0,
        0,
        0,
        23,

        ITOD,
    };
    // after this bytecode the result of 20 + 22 should be on top of the stack

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 2);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, ITOD);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);


    double double_value = flux_object_get_double_value(result);
    ck_assert_double_eq(double_value, 23.0);


    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_STORE)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,

        // Integer 20 in big endian notation 4 byte
        0,
        0,
        0,
        20,

        IPUSH,

        0,
        0,
        0,
        0,

        STORE,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 3);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, STORE);


    flux_vm_execute(vm, code);

    FluxObject* obj = vm->vars[0];

    ck_assert_int_eq(flux_object_get_int_value(obj), 20);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_STORE_and_LOAD)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,

        // Integer 20 in big endian notation 4 byte
        0,
        0,
        0,
        33,

        IPUSH,

        0,
        0,
        0,
        0,

        STORE,

        IPUSH,

        0,
        0,
        0,
        0,

        LOAD,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 5);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, STORE);
    ck_assert_int_eq(commands[3]->instruction, IPUSH);
    ck_assert_int_eq(commands[4]->instruction, LOAD);


    flux_vm_execute(vm, code);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);


    ck_assert_int_eq(flux_object_get_int_value(result), 33);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_JMP)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        // JSR should jump over the next IPUSH therefore there should be 9 on the stack and nothing else
        JSR, // 0

        0,
        0,
        0,
        2,

        IPUSH, // 1

        0,
        0,
        0,
        5,

        IPUSH, // 2 <- jmp should end up here

        0,
        0,
        0,
        9,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 3);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, JSR);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, IPUSH);

    flux_vm_execute(vm, code);

    FluxObject* pushed_integer = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* return_address = flux_stack_get_noffset(vm->stack,2);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack,3);

    ck_assert_ptr_nonnull(pushed_integer);
    ck_assert_ptr_nonnull(return_address);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(return_address), 1); // return address should be the location of JSR + 1
    ck_assert_int_eq(flux_object_get_int_value(pushed_integer), 9);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_CMP_JE)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        5,

        CMP,

        JE,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        25,

        IPUSH,
        0,
        0,
        0,
        20,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 6);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, CMP);
    ck_assert_int_eq(commands[3]->instruction, JE);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);
    ck_assert_int_eq(commands[5]->instruction, IPUSH);

    flux_vm_execute(vm, code);

    FluxObject* pushed_integer = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack,2);

    ck_assert_ptr_nonnull(pushed_integer);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(pushed_integer), 20);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_INSPECT)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,
        0,
        0,
        25,

        INSPECT,
    };
    // after this code there should still be 25 on top of the stack

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 2);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, INSPECT);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_int_value(result), 25);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_JL)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        10,

        CMP,

        JL,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        25,

        IPUSH,
        0,
        0,
        0,
        20,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 6);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, CMP);
    ck_assert_int_eq(commands[3]->instruction, JL);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);
    ck_assert_int_eq(commands[5]->instruction, IPUSH);

    flux_vm_execute(vm, code);

    FluxObject* pushed_integer = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack,2);

    ck_assert_ptr_nonnull(pushed_integer);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(pushed_integer), 20);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_JL_NO_JUMP)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        4,

        CMP,

        JL,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        25,

        IPUSH,
        0,
        0,
        0,
        20,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 6);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, CMP);
    ck_assert_int_eq(commands[3]->instruction, JL);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);
    ck_assert_int_eq(commands[5]->instruction, IPUSH);

    flux_vm_execute(vm, code);

    FluxObject* pushed_integer = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* pushed_integer2 = flux_stack_get_noffset(vm->stack, 2);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack,3);

    ck_assert_ptr_nonnull(pushed_integer);
    ck_assert_ptr_nonnull(pushed_integer2);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(pushed_integer), 20);
    ck_assert_int_eq(flux_object_get_int_value(pushed_integer2), 25);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_JLE)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        6,

        CMP,

        JLE,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        25,

        IPUSH,
        0,
        0,
        0,
        20,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 6);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, CMP);
    ck_assert_int_eq(commands[3]->instruction, JLE);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);
    ck_assert_int_eq(commands[5]->instruction, IPUSH);

    flux_vm_execute(vm, code);

    FluxObject* pushed_integer = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack,2);

    ck_assert_ptr_nonnull(pushed_integer);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(pushed_integer), 20);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_JGE)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,
        0,
        0,
        7,

        IPUSH,
        0,
        0,
        0,
        6,

        CMP,

        JGE,
        0,
        0,
        0,
        5,

        IPUSH,
        0,
        0,
        0,
        25,

        IPUSH,
        0,
        0,
        0,
        20,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 6);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, CMP);
    ck_assert_int_eq(commands[3]->instruction, JGE);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);
    ck_assert_int_eq(commands[5]->instruction, IPUSH);

    flux_vm_execute(vm, code);

    FluxObject* pushed_integer = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack,2);

    ck_assert_ptr_nonnull(pushed_integer);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(pushed_integer), 20);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_SUBTRACT)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,

        // Integer 20 in big endian notation 4 byte
        0,
        0,
        0,
        20,


        IPUSH,

        // Integer 12 in big endian notation 4 byte
        0,
        0,
        0,
        12,

        ISUB,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 3);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, ISUB);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_int_value(result), 8);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_MULTIPLY)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,

        // Integer 5 in big endian notation 4 byte
        0,
        0,
        0,
        5,


        IPUSH,

        // Integer 4 in big endian notation 4 byte
        0,
        0,
        0,
        4,

        IMUL,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 3);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, IMUL);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_int_value(result), 20);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_IDIV)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,

        // Integer 5 in big endian notation 4 byte
        0,
        0,
        0,
        10,


        IPUSH,

        // Integer 4 in big endian notation 4 byte
        0,
        0,
        0,
        2,

        IDIV,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 3);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, IDIV);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_int_value(result), 5);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_PRINT_STRING)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        SPUSH,

        0,
        0,
        0,
        6,
        'H',
        'e',
        'l',
        'l',
        'o',
        '\0',

        INSPECT,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_int_eq(code->number_of_commands, 2);

    FluxCommand** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);

    ck_assert_int_eq(commands[0]->instruction, SPUSH);
    ck_assert_int_eq(commands[1]->instruction, INSPECT);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_ptr_nonnull(result);
    ck_assert_int_eq(flux_object_get_type(result), String);

    ck_assert_int_eq(strcmp(flux_object_get_str_value(result), "Hello"), 0);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_EXCEPTION_IDIV_BY_ZERO)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        // create the exception table
        AEX, // add exception
        0,0,0,0, // valid from instruction 0 (IPUSH)
        0,0,0,2, // up to instruction 2 (IDIV)
        0,0,0,4, // in case there is a throw we jump to Instruction 4 (the last IPUSH)
        0,0,0,0, // flux_exception_type_division_by_zero,

        // Start actual code
        IPUSH,

        0,
        0,
        0,
        6,

        IPUSH,
        0,
        0,
        0,
        0,

        IDIV, // IDIV will internally throw

        IPUSH, // this code should never be run
        0,0,0,20,


        // Code to handle the exception
        IPUSH,
        0,0,0,10
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_ptr_nonnull(code);

    ck_assert_int_eq(code->number_of_commands, 5);

    FluxCommand** commands = code->commands;
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, IDIV);
    ck_assert_int_eq(commands[3]->instruction, IPUSH);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack, 2); // We know the exception was thrown when the first IPUSH after IDIV was not executed

    ck_assert_ptr_nonnull(result);
    ck_assert_ptr_null(should_be_null);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_EXCEPTION_THROW_INTEGER)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        // create the exception table
        AEX,
        0,0,0,0,
        0,0,0,2,
        0,0,0,4,
        0,0,0,1,

        IPUSH,
        0,0,0,6, // this variable should be just be thrown away when the stack gets cleaned up

        IPUSH,
        0,0,0,7, // <- value to be thrown

        THROW,

        IPUSH, // this code should never be run
        0,0,0,20,


        // Code to handle the exception
        IPUSH,
        0,0,0,10,

        IADD // Add 10 to the thrown value (7)
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_ptr_nonnull(code);

    ck_assert_int_eq(code->number_of_commands, 6);

    FluxCommand** commands = code->commands;
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, THROW);
    ck_assert_int_eq(commands[3]->instruction, IPUSH);
    ck_assert_int_eq(commands[4]->instruction, IPUSH);
    ck_assert_int_eq(commands[5]->instruction, IADD);


    flux_vm_execute(vm, code);
    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack, 2); 

    ck_assert_ptr_nonnull(result);
    ck_assert_ptr_null(should_be_null); // since throw empties the stack and only puts the result back onto the stack there should be nothing else left

    ck_assert_int_eq(flux_object_get_int_value(result), 17);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_EXCEPTION_THROW_INTEGER_NO_EXCEPTION_TABLE)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {
        IPUSH,
        0,0,0,6, // this variable should be just be thrown away when the stack gets cleaned up

        IPUSH,
        0,0,0,7, // <- value to be thrown

        THROW,

        IPUSH, // this code should never be run
        0,0,0,20,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_ptr_nonnull(code);

    ck_assert_int_eq(code->number_of_commands, 4);

    FluxCommand** commands = code->commands;
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, THROW);
    ck_assert_int_eq(commands[3]->instruction, IPUSH);


    FluxVMError error = flux_vm_execute(vm, code);

    ck_assert_int_eq(error.type, flux_vm_error_type_uncaught_exception);
    ck_assert_int_eq(error.position, 2);

    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_CALL_FUNCTION_0_ARGUMENTS)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        JSR,
        0,0,0,2,
        EXIT,

        IPUSH,
        0,0,0,6, // this variable should be just be thrown away when the stack gets cleaned up
        RET,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_ptr_nonnull(code);

    ck_assert_int_eq(code->number_of_commands, 4);

    FluxCommand** commands = code->commands;
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);

    ck_assert_int_eq(commands[0]->instruction, JSR);
    ck_assert_int_eq(commands[1]->instruction, EXIT);
    ck_assert_int_eq(commands[2]->instruction, IPUSH);
    ck_assert_int_eq(commands[3]->instruction, RET);


    flux_vm_execute(vm, code);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack, 2);

    ck_assert_ptr_nonnull(result);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(result), 6);
    ck_assert_int_eq(flux_object_get_type(result), Integer);



    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

START_TEST (test_bytecode_CALL_FUNCTION_2_ARGUMENTS)
{
    FluxVM* vm = flux_vm_init();

    char bytecode[] = {

        IPUSH,
        0,0,0,2,

        IPUSH,
        0,0,0,4,

        STORE0,
        STORE1,

        JSR,
        0,0,0,6,
        EXIT,

        LOAD0,
        LOAD1,
        IADD,
        RET,
    };

    FluxCode* code = flux_code_init(bytecode, sizeof(bytecode));

    ck_assert_ptr_nonnull(code);

    ck_assert_int_eq(code->number_of_commands, 10);

    FluxCommand** commands = code->commands;
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);
    ck_assert_ptr_nonnull(commands[3]);
    ck_assert_ptr_nonnull(commands[4]);
    ck_assert_ptr_nonnull(commands[5]);
    ck_assert_ptr_nonnull(commands[6]);
    ck_assert_ptr_nonnull(commands[7]);
    ck_assert_ptr_nonnull(commands[8]);
    ck_assert_ptr_nonnull(commands[9]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, STORE0);
    ck_assert_int_eq(commands[3]->instruction, STORE1);
    ck_assert_int_eq(commands[4]->instruction, JSR);
    ck_assert_int_eq(commands[5]->instruction, EXIT);
    ck_assert_int_eq(commands[6]->instruction, LOAD0);
    ck_assert_int_eq(commands[7]->instruction, LOAD1);
    ck_assert_int_eq(commands[8]->instruction, IADD);
    ck_assert_int_eq(commands[9]->instruction, RET);


    flux_vm_execute(vm, code);

    FluxObject* result = flux_stack_get_noffset(vm->stack, 1);
    FluxObject* should_be_null = flux_stack_get_noffset(vm->stack, 2);

    ck_assert_ptr_nonnull(result);
    ck_assert_ptr_null(should_be_null);

    ck_assert_int_eq(flux_object_get_int_value(result), 6);
    ck_assert_int_eq(flux_object_get_type(result), Integer);



    flux_code_free(code);
    flux_vm_free(vm);
}
END_TEST

TEST_HELPER_START(flux_vm);

TEST_HELPER_ADD_TEST(test_stack_integer_addition);
TEST_HELPER_ADD_TEST(test_stack_integer_subtraction);
TEST_HELPER_ADD_TEST(test_stack_integer_multiplication);
TEST_HELPER_ADD_TEST(test_stack_integer_division);
TEST_HELPER_ADD_TEST(test_stack_integer_division_with_remainder);
TEST_HELPER_ADD_TEST(test_adding_int_variables);
TEST_HELPER_ADD_TEST(test_running_bytecode);
TEST_HELPER_ADD_TEST(test_bytecode_ITOD);
TEST_HELPER_ADD_TEST(test_bytecode_STORE);
TEST_HELPER_ADD_TEST(test_bytecode_STORE_and_LOAD);
TEST_HELPER_ADD_TEST(test_bytecode_JMP);
TEST_HELPER_ADD_TEST(test_bytecode_CMP_JE);
TEST_HELPER_ADD_TEST(test_bytecode_INSPECT);
TEST_HELPER_ADD_TEST(test_bytecode_JL);
TEST_HELPER_ADD_TEST(test_bytecode_JL_NO_JUMP);
TEST_HELPER_ADD_TEST(test_bytecode_JLE);
TEST_HELPER_ADD_TEST(test_bytecode_JGE);
TEST_HELPER_ADD_TEST(test_bytecode_SUBTRACT);
TEST_HELPER_ADD_TEST(test_bytecode_MULTIPLY);
TEST_HELPER_ADD_TEST(test_bytecode_IDIV);
TEST_HELPER_ADD_TEST(test_bytecode_PRINT_STRING);
TEST_HELPER_ADD_TEST(test_bytecode_EXCEPTION_IDIV_BY_ZERO);
TEST_HELPER_ADD_TEST(test_bytecode_EXCEPTION_THROW_INTEGER);
TEST_HELPER_ADD_TEST(test_bytecode_EXCEPTION_THROW_INTEGER_NO_EXCEPTION_TABLE);
TEST_HELPER_ADD_TEST(test_bytecode_CALL_FUNCTION_0_ARGUMENTS);
TEST_HELPER_ADD_TEST(test_bytecode_CALL_FUNCTION_2_ARGUMENTS);

TEST_HELPER_END_TEST
