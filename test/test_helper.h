#include"check.h"

#define TEST_HELPER_FUNC_DEFINE(x) TCase* tests_ ## x (void)

#define TEST_HELPER_START(_TEST_NAME_) TEST_HELPER_FUNC_DEFINE(_TEST_NAME_) { TCase* tc = tcase_create(#_TEST_NAME_)

#define TEST_HELPER_ADD_TEST(_TEST_) tcase_add_test(tc, _TEST_)

#define TEST_HELPER_END_TEST return tc; }
