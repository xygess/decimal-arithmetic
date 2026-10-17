#include <stdlib.h>

#include "test_s21_decimal.h"

int main(void) {
  SRunner* runner = srunner_create(arithmetic_suite());

  srunner_add_suite(runner, comparison_suite());
  srunner_add_suite(runner, conversion_suite());
  srunner_add_suite(runner, other_suite());

  srunner_run_all(runner, CK_NORMAL);

  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
