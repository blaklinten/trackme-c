#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

#include "test_timer.h"
#include "test_db.h"


int main(void) {

  const struct CMUnitTest tests[] = {
      cmocka_unit_test(timer_reset),
      cmocka_unit_test(timer_start),
      cmocka_unit_test(timer_stop),
      cmocka_unit_test(db_connect),
      cmocka_unit_test(db_save),
      cmocka_unit_test(db_get_by_name),
      cmocka_unit_test(db_get_by_client),
      cmocka_unit_test(db_get_by_project),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
