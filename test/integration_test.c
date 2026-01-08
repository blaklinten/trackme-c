#include "integration_test.h"
#include "../src/db.h"
#include "test_db.h"
#include <stdlib.h>

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));

  s->TEST_ACTIVITY_1 = "Alice";
  s->TEST_ACTIVITY_2 = "Bob";
  s->TEST_CLIENT = "test client";
  s->TEST_PROJECT = "test project";
  s->TEST_START_TIME_S = 1721664594;           // Mon 22 Jul 18:09:54 CEST 2024
  s->TEST_END_TIME_S = 1721669586;             // Mon 22 Jul 19:33:06 CEST 2024
  s->TEST_DURATION_S = 60 * 60 + 60 * 23 + 12; // 1h23min12s

  *state = s;

  return 0;
}

static int group_teardown(void **state) {
  test_state_t *s = (test_state_t *)*state;

  if (s) {
    free(s);
  }
  free_db();
  return 0;
}

int main(void) {

  const struct CMUnitTest integration_test[] = {
    cmocka_unit_test(test_db_init),
    cmocka_unit_test(test_db_save),
      // cmocka_unit_test(test_db_insert_and_get),
      // cmocka_unit_test(test_db_save_NULL),
      // cmocka_unit_test(test_db_get_by),
  };

  return cmocka_run_group_tests(integration_test, group_setup, group_teardown);
}
