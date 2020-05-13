#include"check.h"
#include"test_helper.h"
#include"flux_vm.h"
#include"flux_log.h"

START_TEST (test_stack_integer_addition)
{
    flux_vm* vm = flux_vm_init();

    flux_vm_ipush(vm, 5);
    flux_vm_ipush(vm, 23);

    flux_vm_iadd(vm);

    flux_object* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 28);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_adding_int_variables)
{
    flux_vm* vm = flux_vm_init();

    flux_vm_ipush(vm, 354);
    flux_vm_ipush(vm, 0);
    flux_vm_store(vm);
    flux_vm_load(vm, 0);

    flux_vm_ipush(vm, 6);

    flux_vm_iadd(vm);

    flux_object* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_type(result), Integer);
    ck_assert_int_eq(flux_object_get_int_value(result), 360);

    flux_vm_free(vm);
}
END_TEST

START_TEST (test_running_bytecode)
{
    flux_vm* vm = flux_vm_init();

    char bytecode[11] = {
        //IPUSH
        3,

        // Integer 20 in big endian notation 4 byte
        0,
        0,
        0,
        20,


        //IPUSH
        3,

        // Integer 22 in big endian notation 4 byte
        0,
        0,
        0,
        22,

        //IADD
        1,
    };
    // after this bytecode the result of 20 + 22 should be on top of the stack

    flux_code* code = flux_code_init(bytecode, 11);

    ck_assert_int_eq(code->number_of_commands, 3);

    flux_command** commands = code->commands;
    
    ck_assert_ptr_nonnull(commands);
    ck_assert_ptr_nonnull(commands[0]);
    ck_assert_ptr_nonnull(commands[1]);
    ck_assert_ptr_nonnull(commands[2]);

    ck_assert_int_eq(commands[0]->instruction, IPUSH);
    ck_assert_int_eq(commands[1]->instruction, IPUSH);
    ck_assert_int_eq(commands[2]->instruction, IADD);


    flux_vm_execute(vm, code);
    flux_object* result = flux_stack_get_noffset(vm->stack, 1);

    ck_assert_int_eq(flux_object_get_int_value(result), 42);


    flux_vm_free(vm);
}
END_TEST

TEST_HELPER_START(flux_vm);

TEST_HELPER_ADD_TEST(test_stack_integer_addition);
TEST_HELPER_ADD_TEST(test_adding_int_variables);
TEST_HELPER_ADD_TEST(test_running_bytecode);

TEST_HELPER_END_TEST
