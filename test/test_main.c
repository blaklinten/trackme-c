#include "test_main.h"
#include "../src/db.h"
#include "test_db.h"
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

  s->KEY = "First Key";
  s->VALUE = "First Value";
  s->test_document = bson_new();

  BSON_APPEND_UTF8(s->test_document, s->KEY, s->VALUE);

  *state = s;

  const char *uri_string = "mongodb://blaklinten:test-psw@localhost:27017";
  init_db(uri_string);
  return 0;
}

static int group_teardown(void **state) {
  test_state_t *test_state = (test_state_t *)*state;

  if (test_state) {
    if (test_state->test_document) {
      bson_destroy(test_state->test_document);
    }
    if (test_state->default_test_info) {
      free(test_state->default_test_info);
    }
    free(test_state);
  }

  free_db();

  return 0;
}

int main(void) {

  const struct CMUnitTest tests[] = {
      cmocka_unit_test(timer_reset),
      cmocka_unit_test(timer_start),
      cmocka_unit_test(timer_stop),
      cmocka_unit_test(timer_get_duration),
      cmocka_unit_test(timer_get_name),
      cmocka_unit_test(timer_get_client),
      cmocka_unit_test(timer_get_project),
      cmocka_unit_test(timer_get_description),
      cmocka_unit_test(db_connect),
  };

  return cmocka_run_group_tests(tests, group_setup, group_teardown);
}
