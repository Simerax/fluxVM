#include"check.h"
#include"test_helper.h"
#include"flux_list.h"

START_TEST(test_list)
{
    flux_list* l = flux_list_init(sizeof(int*));

    int num_a = 2;
    int num_b = 3;
    flux_list_add(l, &num_a);
    flux_list_add(l, &num_b);

    ck_assert_int_eq(flux_list_length(l), 2);

    int** arr = (int**)flux_list_to_array(l);
    ck_assert_int_eq(*arr[0], 2);
    ck_assert_int_eq(*arr[1], 3);

    flux_list_free(l);
}

TEST_HELPER_START(flux_list);

TEST_HELPER_ADD_TEST(test_list);

TEST_HELPER_END_TEST
