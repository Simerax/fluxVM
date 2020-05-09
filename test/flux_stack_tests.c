#include"check.h"
#include"test_helper.h"
#include"flux_stack.h"

START_TEST (test_stack_push_pop)
{
    flux_stack* stack = flux_stack_init();
    ck_assert_int_eq(flux_stack_get_error(stack), flux_stack_error_no_error);

    flux_stack_ipush(stack, 23);
    flux_stack_pop(stack);

    flux_stack_free(stack);

}
END_TEST

START_TEST (test_stack_underflow)
{
    flux_stack* stack = flux_stack_init();

    flux_stack_ipush(stack, 23);
    flux_stack_pop(stack);

    ck_assert_int_eq(flux_stack_get_error(stack), flux_stack_error_no_error);

    flux_stack_pop(stack);

    ck_assert_int_eq(flux_stack_get_error(stack), flux_stack_error_underflow);

    flux_stack_reset_error(stack);

    ck_assert_int_eq(flux_stack_get_error(stack), flux_stack_error_no_error);

    flux_object* underflow_access_obj = flux_stack_get_noffset(stack, 1);

    ck_assert_ptr_null(underflow_access_obj);
    ck_assert_int_eq(flux_stack_get_error(stack), flux_stack_error_underflow_access);

    flux_stack_free(stack);

}
END_TEST

START_TEST (test_stack_overflow)
{
    flux_stack* stack = flux_stack_init();

    for(int i = 0; i <= FLUX_STACK_SIZE; i++)
        flux_stack_ipush(stack, 10);

    ck_assert_int_eq(flux_stack_get_error(stack), flux_stack_error_overflow);


    flux_stack_free(stack);

}
END_TEST


TEST_HELPER_START(flux_stack);

TEST_HELPER_ADD_TEST(test_stack_push_pop);
TEST_HELPER_ADD_TEST(test_stack_underflow);
TEST_HELPER_ADD_TEST(test_stack_overflow);

TEST_HELPER_END_TEST
