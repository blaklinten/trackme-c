#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

#include "test_timer.h"


int main(void) {

  const struct CMUnitTest tests[] = {
      cmocka_unit_test(timer_reset_test),
      cmocka_unit_test(timer_start_test),
      cmocka_unit_test(timer_stop_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
