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

  StartInfo *si_2 = malloc(sizeof(StartInfo));
  si_2->activity = "another_test_activity_name";
  // client same as si
  si_2->client = "test_client";
  si_2->project = "another_test_project";
  si_2->description = "Another description with special charactes: 1!|>åäö`^~'´([/";
  TimerResult *tr_2 = malloc(sizeof(TimerResult));
  tr_2->info = si_2;
  tr_2->start_time = 1721668194;           // Mon 22 Jul 19:09:54 CEST 2024
  tr_2->end_time = 1721673003;             // Mon 22 Jul 20:30:03 CEST 2024
  tr_2->duration = 60 * 60 + 60 * 23 + 12; // 1h20min0s
  s->TEST_TIMER_RESULT_2 = tr_2;

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
    if (s->TEST_TIMER_RESULT_2) {
     free(s->TEST_TIMER_RESULT_2->info);
     free(s->TEST_TIMER_RESULT_2);
    }
    sqlite3_close_v2(s->test_handle);
    free(s);
  }
  return 0;
}

static int _reset_db(void **state) {
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *db_handle = s->test_handle;
  char *errMsg;
  const char *drop_table_query =
      "DROP TABLE IF EXISTS " TRACKME_DB_TABLE_TIMER_RESULT ";";
  if (sqlite3_exec(db_handle, drop_table_query, NULL, NULL, &errMsg) !=
      SQLITE_OK) {
    t_log(ERROR, __func__, "Could not reset db: %s", errMsg);
    sqlite3_free(errMsg);
    return 1;
  }
  return 0;
}

int main(void) {

  // clang-format off
  const struct CMUnitTest integration_test[] = {
    cmocka_unit_test_teardown(test_db_init, _reset_db),
    cmocka_unit_test_teardown(test_db_save, _reset_db),
    cmocka_unit_test_teardown(test_db_save_NULL, _reset_db),
    cmocka_unit_test_teardown(test_db_get_by_activity, _reset_db),
    cmocka_unit_test_teardown(test_db_get_by_client, _reset_db),
    cmocka_unit_test_teardown(test_db_get_by_project, _reset_db),
    cmocka_unit_test_teardown(test_db_get_by_null_value, _reset_db),
    cmocka_unit_test_teardown(test_db_get_by_null_key, _reset_db),
  };
  // clang-format on

  return cmocka_run_group_tests(integration_test, group_setup, group_teardown);
}
