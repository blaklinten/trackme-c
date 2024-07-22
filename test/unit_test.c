#include "unit_test.h"
#include "test_timer.h"
#include <stdlib.h>

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));
  s->TEST_START_TIME_MS = 1690876956; // Tue  1 Aug 10:02:36 CEST 2023
  s->TEST_END_TIME_MS = 1690879255;   // Tue  1 Aug 10:40:55 CEST 2023
  StartInfo *si = malloc(sizeof(StartInfo));
  si->name = "test_name";
  si->client = "test_client";
  si->project = "test_project";
  si->description = "this is a test description";
  s->default_test_info = si;
  s->TEST_NAME_1 = "Lucas";
  s->TEST_NAME_2 = "Jonas";

  *state = s;

  return 0;
}

static int group_teardown(void **state) {
  test_state_t *test_state = (test_state_t *)*state;

  if (test_state) {
    if (test_state->default_test_info) {
      free(test_state->default_test_info);
    }
    free(test_state);
  }

  return 0;
}

int main(void) {

  const struct CMUnitTest unit_test[] = {
      cmocka_unit_test(test_timer_reset),
      cmocka_unit_test(test_timer_start),
      cmocka_unit_test(test_timer_stop),
      cmocka_unit_test(test_timer_get_duration),
      cmocka_unit_test(test_timer_get_name),
      cmocka_unit_test(test_timer_get_client),
      cmocka_unit_test(test_timer_get_project),
      cmocka_unit_test(test_timer_get_description),
  };

  return cmocka_run_group_tests(unit_test, group_setup, group_teardown);
}
