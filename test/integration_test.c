#include "../src/db.h"
#include "../src/timer.h"
#include "integration_test.h"
#include "test_db.h"
#include <stdlib.h>

Timer t;

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));

  StartInfo *si = malloc(sizeof(StartInfo));
  si->activity = "test_activity_name";
  si->client = "test_client";
  si->project = "test_project";
  si->description = "this is a test description";
  s->default_start_info = si;

  start(&t, copy_start_info(si));
  s->TEST_TIMER_RESULT = stop(&t);

  sqlite3_open_v2(TRACKME_DB_FILENAME, &s->test_handle,
                  SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
  *state = s;
  return 0;
}

static int group_teardown(void **state) {
  test_state_t *s = (test_state_t *)*state;

  char *errMsg;
  const char *drop_table_sql =
      "DROP TABLE IF EXISTS " TRACKME_DB_TABLE_TIMER_RESULT ";";
  sqlite3_exec(s->test_handle, drop_table_sql, NULL, NULL, &errMsg);
  sqlite3_free(errMsg);

  if (s) {
    if (s->default_start_info) {
      free(s->default_start_info);
    }
    if (s->TEST_TIMER_RESULT) {
      free_timer_result(s->TEST_TIMER_RESULT);
    }
    sqlite3_close_v2(s->test_handle);
    free(s);
  }
  return 0;
}

int main(void) {

  // clang-format off
  const struct CMUnitTest integration_test[] = {
      cmocka_unit_test(test_db_save),
      // cmocka_unit_test(test_db_insert_and_get),
      // cmocka_unit_test(test_db_save_NULL),
      // cmocka_unit_test(test_db_get_by),
    cmocka_unit_test(test_db_init),
  };
  // clang-format on

  return cmocka_run_group_tests(integration_test, group_setup, group_teardown);
}
