#include"check.h"
#include "flux_cmp_result.h"
#include"test_helper.h"
#include"flux_object.h"

START_TEST (test_integer_flux_object)
{
    FluxObject* obj = flux_object_iinit(5);

    ck_assert_int_eq(obj->type, Integer);
    ck_assert_int_eq(flux_object_get_int_value(obj), 5);

    flux_object_free(obj);

}
END_TEST

START_TEST (test_integer_to_double_flux_object)
{
    FluxObject* obj = flux_object_iinit(5);

    ck_assert_int_eq(flux_object_get_type(obj), Integer);
    ck_assert_int_eq(flux_object_itod(obj), 1);

    ck_assert_double_eq(flux_object_get_double_value(obj), 5.0);
    ck_assert_int_eq(flux_object_get_type(obj), Double);


    flux_object_free(obj);

}
END_TEST

START_TEST (test_char_flux_object)
{
    FluxObject* obj = flux_object_cinit('a');

    ck_assert_int_eq(flux_object_get_type(obj), Char);
    ck_assert_int_eq(flux_object_get_char_value(obj), 'a');

    flux_object_free(obj);
}
END_TEST

START_TEST (test_compare_char_flux_object)
{
    FluxObject* obj_a = flux_object_cinit('a');
    FluxObject* obj_b = flux_object_cinit('a');

    ck_assert_int_eq(flux_object_get_char_value(obj_a), flux_object_get_char_value(obj_b));

    FluxCmpResult result = flux_object_cmp(obj_a, obj_b);
    ck_assert_int_eq(result, EQUAL);

    flux_object_free(obj_a);
    flux_object_free(obj_b);

    obj_a = flux_object_cinit('a');
    obj_b = flux_object_cinit('b');

    result = flux_object_cmp(obj_a, obj_b);
    ck_assert_int_eq(result, LESS);

    flux_object_free(obj_a);
    flux_object_free(obj_b);
}
END_TEST

START_TEST (test_str_flux_object)
{
    char string[] = "Hello";
    FluxObject* obj = flux_object_strinit(string, sizeof(string));

    ck_assert_int_eq(flux_object_get_type(obj), String);
    char* value = flux_object_get_str_value(obj);
    ck_assert_int_eq(strcmp(value, "Hello"), 0);
    ck_assert_int_eq(flux_object_get_size(obj), 6); // 6 because it's NULL terminated

    flux_object_free(obj);
}
END_TEST


TEST_HELPER_START(flux_object);

TEST_HELPER_ADD_TEST(test_integer_flux_object);
TEST_HELPER_ADD_TEST(test_integer_to_double_flux_object);
TEST_HELPER_ADD_TEST(test_char_flux_object);
TEST_HELPER_ADD_TEST(test_compare_char_flux_object);
TEST_HELPER_ADD_TEST(test_str_flux_object);

TEST_HELPER_END_TEST
