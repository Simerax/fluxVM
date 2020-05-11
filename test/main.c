#include<stdlib.h>
#include"check.h"
#include"test_helper.h"


/* Test cases
 * Don't forget to add the testcase to the main suite
 * */
TEST_HELPER_FUNC_DEFINE(flux_object);
TEST_HELPER_FUNC_DEFINE(flux_stack);
TEST_HELPER_FUNC_DEFINE(flux_vm);
TEST_HELPER_FUNC_DEFINE(flux_list);



/* Main Test Suite */ 
Suite* flux_test_suite(void) {
    Suite* s;
    s = suite_create("fluxVMSuite");

    suite_add_tcase(s, tests_flux_object());
    suite_add_tcase(s, tests_flux_stack());
    suite_add_tcase(s, tests_flux_vm());
    suite_add_tcase(s, tests_flux_list());

    return s;
}

int main(void) {

    int number_failed;
    SRunner *sr;

    sr = srunner_create(flux_test_suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
