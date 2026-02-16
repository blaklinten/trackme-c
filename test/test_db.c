#include "test_db.h"
#include "../src/db.h"
#include "../src/util/log.h"
#include "../src/util/timer_result_list.h"
#include "integration_test.h"
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>

/*** Private helper functions ***/

bool _compare_timerresults(TimerResult *first, TimerResult *second) {
  char *first_activity = first->info->activity;
  char *second_activity = second->info->activity;
  if (strcmp(first_activity, second_activity)) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_activity, second_activity);
    return false;
  }

  char *first_client = first->info->client;
  char *second_client = second->info->client;
  if (strcmp(first_client, second_client)) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_client, second_client);
    return false;
  }
  char *first_project = first->info->project;
  char *second_project = second->info->project;
  if (strcmp(first_project, second_project)) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_project, second_project);
    return false;
  }
  char *first_description = first->info->description;
  char *second_description = second->info->description;
  if (strcmp(first_description, second_description)) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_description,
          second_description);
    return false;
  }
  int first_start_time = first->start_time;
  int second_start_time = second->start_time;
  if (first_start_time != second_start_time) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_start_time,
          second_start_time);
    return false;
  }
  int first_end_time = first->end_time;
  int second_end_time = second->end_time;
  if (first_end_time != second_end_time) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_end_time, second_end_time);
    return false;
  }
  int first_duration = first->duration;
  int second_duration = second->duration;
  if (first_duration != second_duration) {
    t_log(ERROR, __func__, "[%s] is not [%s]", first_duration, second_duration);
    return false;
  }
  return true;
}

bool _insert_test_data(test_state_t *s) {
  sqlite3 *test_db_insert_handle = NULL;
  sqlite3_stmt *insert_stmt = NULL, *insert_stmt_2 = NULL;

  // clang-format off
  const char *insert_query =
      "INSERT INTO " TRACKME_DB_TABLE_TIMER_RESULT "("
      DB_KEY_ACTIVITY ", "
      DB_KEY_CLIENT ", "
      DB_KEY_PROJECT ", "
      DB_KEY_DESCRIPTION ", "
      DB_KEY_START_TIME ", "
      DB_KEY_END_TIME ", "
      DB_KEY_DURATION 
      ") VALUES ("
      "?,?,?,?,?,?,?"
      ")";
  // clang-format on

  if (sqlite3_open_v2(TRACKME_DB_FILENAME, &test_db_insert_handle,
                      SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK) {
    t_log(ERROR, __func__, "Could not open db: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  }
  // Prepare first insert
  if (sqlite3_prepare(test_db_insert_handle, insert_query, -1, &insert_stmt,
                      NULL) != SQLITE_OK) {
    t_log(ERROR, __func__, "Could not prepare insert statement: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  }

  if (sqlite3_bind_text(insert_stmt, 1, s->TEST_TIMER_RESULT->info->activity,
                        -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 2, s->TEST_TIMER_RESULT->info->client, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 3, s->TEST_TIMER_RESULT->info->project, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 4, s->TEST_TIMER_RESULT->info->description,
                        -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_int(insert_stmt, 5, s->TEST_TIMER_RESULT->start_time) !=
          SQLITE_OK ||
      sqlite3_bind_int(insert_stmt, 6, s->TEST_TIMER_RESULT->end_time) !=
          SQLITE_OK ||
      sqlite3_bind_int(insert_stmt, 7, s->TEST_TIMER_RESULT->duration) !=
          SQLITE_OK) {
    t_log(ERROR, __func__, "Could not bind parameters to insert statement: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  };

  if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
    t_log(ERROR, __func__, "Could not execute insert statement: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  }

  // Prepare second insert
  if (sqlite3_prepare(test_db_insert_handle, insert_query, -1, &insert_stmt_2,
                      NULL) != SQLITE_OK) {
    t_log(ERROR, __func__, "Could not prepare insert statement: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  }

  if (sqlite3_bind_text(insert_stmt_2, 1,
                        s->TEST_TIMER_RESULT_2->info->activity, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt_2, 2, s->TEST_TIMER_RESULT_2->info->client,
                        -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt_2, 3, s->TEST_TIMER_RESULT_2->info->project,
                        -1, SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt_2, 4,
                        s->TEST_TIMER_RESULT_2->info->description, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_int(insert_stmt_2, 5, s->TEST_TIMER_RESULT_2->start_time) !=
          SQLITE_OK ||
      sqlite3_bind_int(insert_stmt_2, 6, s->TEST_TIMER_RESULT_2->end_time) !=
          SQLITE_OK ||
      sqlite3_bind_int(insert_stmt_2, 7, s->TEST_TIMER_RESULT_2->duration) !=
          SQLITE_OK) {
    t_log(ERROR, __func__, "Could not bind parameters to insert statement: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  };

  if (sqlite3_step(insert_stmt_2) != SQLITE_DONE) {
    t_log(ERROR, __func__, "Could not execute insert statement: %s",
          sqlite3_errmsg(test_db_insert_handle));
    return false;
  }

  sqlite3_close_v2(test_db_insert_handle);
  sqlite3_finalize(insert_stmt);
  sqlite3_finalize(insert_stmt_2);
  return true;
}

/*** Tests begin ***/

void test_db_init(void **state) {
  (void)state;
  // Given
  sqlite3 *test_db_init_handle = NULL;
  const char *contents_query = "SELECT * FROM " TRACKME_DB_TABLE_TIMER_RESULT;
  sqlite3_stmt *table_contents_stmt;
  // clang-format off
  char *db_keys[NUMBER_OF_COLUMS] = {
    DB_KEY_ACTIVITY,
    DB_KEY_CLIENT,
    DB_KEY_PROJECT,
    DB_KEY_DESCRIPTION,
    DB_KEY_START_TIME,
    DB_KEY_END_TIME,
    DB_KEY_DURATION
  };
  // clang-format on

  // When
  bool initialized = init_db();

  // Then
  assert_true(initialized);
  assert_int_equal(sqlite3_open_v2(TRACKME_DB_FILENAME, &test_db_init_handle,
                                   SQLITE_OPEN_READWRITE, NULL),
                   SQLITE_OK);
  assert_int_equal(sqlite3_prepare_v2(test_db_init_handle, contents_query, -1,
                                      &table_contents_stmt, NULL),
                   SQLITE_OK);
  assert_int_equal(sqlite3_step(table_contents_stmt), SQLITE_DONE);
  assert_int_equal(sqlite3_column_count(table_contents_stmt), NUMBER_OF_COLUMS);

  for (int i = 0; i < NUMBER_OF_COLUMS; i++) {
    assert_string_equal(db_keys[i],
                        sqlite3_column_name(table_contents_stmt, i));
  }

  // Finally
  sqlite3_finalize(table_contents_stmt);
  sqlite3_close(test_db_init_handle);
  free_db();
};

void test_db_save(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *test_db_save_handle = NULL;
  sqlite3_stmt *table_contents_stmt;
  const char *contents_query = "SELECT * FROM " TRACKME_DB_TABLE_TIMER_RESULT;
  char **errmsg = NULL;
  // clang-format off
  char *expected_db_contents_string[4] = {
    s->TEST_TIMER_RESULT->info->activity,
    s->TEST_TIMER_RESULT->info->client,
    s->TEST_TIMER_RESULT->info->project,
    s->TEST_TIMER_RESULT->info->description
  };

  time_t expected_db_contents_time[3] = {
    s->TEST_TIMER_RESULT->start_time,
    s->TEST_TIMER_RESULT->end_time,
    s->TEST_TIMER_RESULT->duration
  };
  // clang-format on
  init_db();

  // When
  bool sucess = save(s->TEST_TIMER_RESULT);

  // Then
  assert_true(sucess);
  assert_int_equal(sqlite3_open_v2(TRACKME_DB_FILENAME, &test_db_save_handle,
                                   SQLITE_OPEN_READWRITE, NULL),
                   SQLITE_OK);
  assert_int_equal(sqlite3_prepare_v2(test_db_save_handle, contents_query, -1,
                                      &table_contents_stmt, NULL),
                   SQLITE_OK);
  assert_int_equal(sqlite3_column_count(table_contents_stmt), NUMBER_OF_COLUMS);
  assert_int_equal(sqlite3_step(table_contents_stmt), SQLITE_ROW);

  for (int i = 0; i < 4; i++) {
    assert_string_equal(expected_db_contents_string[i],
                        (char *)sqlite3_column_text(table_contents_stmt, i));
  }

  for (int i = 0; i < 3; i++) {
    assert_int_equal(expected_db_contents_time[i],
                     sqlite3_column_int(table_contents_stmt, i + 4));
  }
  assert_int_equal(sqlite3_step(table_contents_stmt), SQLITE_DONE);

  // Finally
  sqlite3_finalize(table_contents_stmt);
  sqlite3_close(test_db_save_handle);
  sqlite3_free(errmsg);
  free_db();
}

void test_db_save_NULL(void **state) {
  (void)state;
  // Given
  sqlite3 *test_db_save_handle = NULL;
  sqlite3_stmt *table_contents_stmt;
  const char *contents_query = "SELECT * FROM " TRACKME_DB_TABLE_TIMER_RESULT;
  char **errmsg = NULL;

  init_db();

  // When
  bool sucess = save(NULL);

  // Then
  assert_false(sucess);
  assert_int_equal(sqlite3_open_v2(TRACKME_DB_FILENAME, &test_db_save_handle,
                                   SQLITE_OPEN_READWRITE, NULL),
                   SQLITE_OK);
  assert_int_equal(sqlite3_prepare_v2(test_db_save_handle, contents_query, -1,
                                      &table_contents_stmt, NULL),
                   SQLITE_OK);
  assert_int_equal(sqlite3_column_count(table_contents_stmt), NUMBER_OF_COLUMS);
  // SQLITE_DONE means no result to process i.e. we did not save anything :)
  assert_int_equal(sqlite3_step(table_contents_stmt), SQLITE_DONE);

  // Finally
  sqlite3_finalize(table_contents_stmt);
  sqlite3_close(test_db_save_handle);
  sqlite3_free(errmsg);
  free_db();
}

void test_db_get_by_activity(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *test_db_insert_handle = NULL;
  char **errmsg = NULL;

  init_db();
  _insert_test_data(s);

  // When
  timer_result_list *one_result =
      get_by(DB_KEY_ACTIVITY, s->TEST_TIMER_RESULT->info->activity);
  timer_result_list *no_result =
      get_by(DB_KEY_ACTIVITY, "Not-a-match");

  // Then
  assert_non_null(one_result);
  assert_non_null(no_result);
  assert_int_equal(count_elements(one_result), 1);
  assert_true(_compare_timerresults(one_result->value, s->TEST_TIMER_RESULT));
  assert_int_equal(count_elements(no_result), 0);

  // Finally
  free_list(one_result);
  free_list(no_result);
  sqlite3_close(test_db_insert_handle);
  sqlite3_free(errmsg);
  free_db();
}

void test_db_get_by_client(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *test_db_insert_handle = NULL;
  char **errmsg = NULL;

  init_db();
  // Insert TEST_TIMER_RESULT and TEST_TIMER_RESULT_2 into db
  _insert_test_data(s);

  // When
  timer_result_list *two_results =
      get_by(DB_KEY_CLIENT, s->TEST_TIMER_RESULT->info->client);
  timer_result_list *no_result =
      get_by(DB_KEY_CLIENT, "Not-a-match");

  // Then
  assert_non_null(two_results);
  assert_non_null(no_result);
  // The client is same for TEST_TIMER_RESULT and TEST_TIMER_RESULT_2
  assert_int_equal(count_elements(two_results), 2);
  assert_true(_compare_timerresults(two_results->value, s->TEST_TIMER_RESULT));
  assert_true(_compare_timerresults(two_results->next->value, s->TEST_TIMER_RESULT_2));
  assert_int_equal(count_elements(no_result), 0);

  // Finally
  free_list(two_results);
  free_list(no_result);
  sqlite3_close(test_db_insert_handle);
  sqlite3_free(errmsg);
  free_db();
}

void test_db_get_by_project(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *test_db_insert_handle = NULL;
  char **errmsg = NULL;

  init_db();
  _insert_test_data(s);

  // When
  timer_result_list *one_result =
      get_by(DB_KEY_PROJECT, s->TEST_TIMER_RESULT->info->project);
  timer_result_list *no_result =
      get_by(DB_KEY_PROJECT, "Not-a-match");

  // Then
  assert_non_null(one_result);
  assert_non_null(no_result);
  assert_int_equal(count_elements(one_result), 1);
  assert_true(_compare_timerresults(one_result->value, s->TEST_TIMER_RESULT));
  assert_int_equal(count_elements(no_result), 0);

  // Finally
  free_list(one_result);
  free_list(no_result);
  sqlite3_close(test_db_insert_handle);
  sqlite3_free(errmsg);
  free_db();
}

void test_db_get_by_null_key(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *test_db_insert_handle = NULL;
  char **errmsg = NULL;

  init_db();
  _insert_test_data(s);

  // When
  timer_result_list *no_result =
      get_by(NULL, s->TEST_TIMER_RESULT->info->project);

  // Then
  assert_null(no_result);

  // Finally
  sqlite3_close(test_db_insert_handle);
  sqlite3_free(errmsg);
  free_db();
}

void test_db_get_by_null_value(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  sqlite3 *test_db_insert_handle = NULL;
  char **errmsg = NULL;

  init_db();
  _insert_test_data(s);

  // When
  timer_result_list *no_result =
      get_by(DB_KEY_PROJECT, NULL);

  // Then
  assert_null(no_result);

  // Finally
  sqlite3_close(test_db_insert_handle);
  sqlite3_free(errmsg);
  free_db();
}
