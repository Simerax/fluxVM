#include"check.h"
#include"test_helper.h"

START_TEST (my_stupid_first_test)
{
    ck_assert_int_eq(5, 5);
}
END_TEST


TEST_HELPER_START(flux_object);

TEST_HELPER_ADD_TEST(my_stupid_first_test);

TEST_HELPER_END_TEST
