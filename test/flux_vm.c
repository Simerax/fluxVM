#include"check.h"
#include"test_helper.h"
#include"flux_vm.h"

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


TEST_HELPER_START(flux_vm);

TEST_HELPER_ADD_TEST(test_stack_integer_addition);
TEST_HELPER_ADD_TEST(test_adding_int_variables);

TEST_HELPER_END_TEST
