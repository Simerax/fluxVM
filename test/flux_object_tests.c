#include"check.h"
#include"test_helper.h"
#include"flux_object.h"

START_TEST (test_integer_flux_object)
{
    flux_object* obj = flux_object_iinit(5);

    ck_assert_int_eq(obj->type, Integer);
    ck_assert_int_eq(flux_object_get_int_value(obj), 5);

    flux_object_free(obj);

}
END_TEST


TEST_HELPER_START(flux_object);

TEST_HELPER_ADD_TEST(test_integer_flux_object);

TEST_HELPER_END_TEST
