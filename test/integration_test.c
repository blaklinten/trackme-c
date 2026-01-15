#include "integration_test.h"
#include "../src/db.h"
#include "../src/timer.h"
#include "../src/util/log.h"
#include "test_db.h"
#include <stdlib.h>

Timer t;

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));

  StartInfo *si = malloc(sizeof(StartInfo));
  si->activity = "test_activity_name";
  si->client = "test_client";
  si->project = "test_project";
  si->description = "Description with special charactes: 1!|>åäö`^~'´([/";
  TimerResult *tr = malloc(sizeof(TimerResult));
  tr->info = si;
  tr->start_time = 1721664594;           // Mon 22 Jul 18:09:54 CEST 2024
  tr->end_time = 1721669586;             // Mon 22 Jul 19:33:06 CEST 2024
  tr->duration = 60 * 60 + 60 * 23 + 12; // 1h23min12s
  s->TEST_TIMER_RESULT = tr;

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
  if ( sqlite3_exec(s->test_handle, drop_table_sql, NULL, NULL, &errMsg) != SQLITE_OK) {
    t_log(ERROR, __func__, "Could not reset db: %s", errMsg);
  }
  sqlite3_free(errMsg);

  if (s) {
    if (s->TEST_TIMER_RESULT) {
     free(s->TEST_TIMER_RESULT->info);
     free(s->TEST_TIMER_RESULT);
    }
    sqlite3_close_v2(s->test_handle);
    free(s);
  }
  return 0;
}

int main(void) {

  // clang-format off
  const struct CMUnitTest integration_test[] = {
    cmocka_unit_test(test_db_init),
    cmocka_unit_test(test_db_save),
    cmocka_unit_test(test_db_save_NULL),
    // cmocka_unit_test(test_db_insert_and_get),
    // cmocka_unit_test(test_db_get_by),
  };
  // clang-format on

  return cmocka_run_group_tests(integration_test, group_setup, group_teardown);
}
