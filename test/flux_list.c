#include"check.h"
#include"test_helper.h"
#include"flux_list.h"
#include<stdlib.h> // malloc, free

START_TEST(test_list)
{
    FluxList* l = flux_list_init(sizeof(int*), NULL);

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

// just a dummy struct to be able to have a more complex freeing scenario
struct str {
    char* content;
};

unsigned int test_list_free_free_func_called = 0;
void test_list_free_str_free_func(void* t) {
    struct str* e = (struct str*)t;
    test_list_free_free_func_called = 1;
    free(e->content);
}

START_TEST(test_list_free)
{
    FluxList* l = flux_list_init(sizeof(struct str*), test_list_free_str_free_func);

    struct str* e = malloc(sizeof(struct str));
    e->content = malloc(sizeof(char));

    flux_list_add(l, e);

    ck_assert_int_eq(flux_list_length(l), 1);

    flux_list_free(l);

    ck_assert_uint_eq(test_list_free_free_func_called, 1);
}


#define TEST_BIG_LIST_FREE_LIST_SIZE 70000
START_TEST(test_big_list_free) 
{
    FluxList* l = flux_list_init(sizeof(struct str*), NULL);

    for(int i = 0; i < TEST_BIG_LIST_FREE_LIST_SIZE; i++) {
        struct str* e = malloc(sizeof(struct str));
        flux_list_add(l, e);
    }

    ck_assert_int_eq(flux_list_length(l), TEST_BIG_LIST_FREE_LIST_SIZE);
    flux_list_free(l);

}

TEST_HELPER_START(flux_list);

TEST_HELPER_ADD_TEST(test_list);
TEST_HELPER_ADD_TEST(test_list_free);
tcase_set_timeout(tc, 30); // well the malloc/free calls for the big list take a few seconds - not perfect but okay for now
TEST_HELPER_ADD_TEST(test_big_list_free);

TEST_HELPER_END_TEST
