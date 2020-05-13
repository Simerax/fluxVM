#include"check.h"
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


TEST_HELPER_START(flux_object);

TEST_HELPER_ADD_TEST(test_integer_flux_object);
TEST_HELPER_ADD_TEST(test_integer_to_double_flux_object);

TEST_HELPER_END_TEST
