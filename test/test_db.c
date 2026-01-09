#include "test_db.h"
#include "../src/db.h"
#include "../src/util/log.h"
#include "integration_test.h"
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>

/*** Private helper functions ***/

/*** Tests begin ***/

void test_db_init(void **state) {
  (void)state;
  // Given
  sqlite3 *test_db_init_handle = NULL;
  const char *query = "SELECT * FROM " TRACKME_DB_TABLE_TIMER_RESULT;
  sqlite3_stmt *sql;

  // When
  bool initialized = init_db();

  // Then
  assert_true(initialized);
  assert_int_equal(sqlite3_open_v2(TRACKME_DB_FILENAME, &test_db_init_handle,
                                   SQLITE_OPEN_READONLY, NULL),
                   SQLITE_OK);
  assert_int_equal(
      sqlite3_prepare_v2(test_db_init_handle, query, -1, &sql, NULL),
      SQLITE_OK);
  assert_int_equal(sqlite3_step(sql), SQLITE_DONE);
  assert_int_equal(sqlite3_column_count(sql), NUMBER_OF_COLUMS);

  // Finally
  sqlite3_finalize(sql);
  sqlite3_close(test_db_init_handle);
  free_db();
};

void test_db_save(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  bool sucess = save(s->TEST_TIMER_RESULT);

  // Then
  assert_true(sucess);

  // Finally
}

void test_db_save_NULL(void **state) {
  // (void) state;
  // // Given
  // // When
  // bool failed_null = save(NULL);

  // // Then
  // assert_false(failed_null);

  // // Finally
}

void test_db_insert_and_get(void **state) {
  // // Given
  // test_state_t *s = (test_state_t *)*state;
  // bson_t *doc1 = bson_new();
  // BSON_APPEND_UTF8(doc1, DB_KEY_ACTIVITY, s->TEST_ACTIVITY_1);

  // bson_t *doc2 = bson_new();
  // BSON_APPEND_UTF8(doc2, DB_KEY_ACTIVITY, s->TEST_ACTIVITY_2);

  // if (!entries) {
  //   t_log(ERROR, __func__, "No collection");
  //   fail();
  // }

  // bson_t reply;
  // bson_error_t error;

  // // When
  // if (!mongoc_collection_insert_one(entries, doc1, NULL, &reply, &error)) {
  //   char *bson_as_json = bson_as_canonical_extended_json(doc1, NULL);
  //   t_log(ERROR, __func__,
  //         "failed to insert document: %s\n"
  //         "error code: [%d]\n"
  //         "error message: [%s]\n"
  //         "error reply: [%s]\n",
  //         bson_as_json, error.code, error.message,
  //         bson_as_canonical_extended_json(&reply, NULL));
  //   bson_free(bson_as_json);
  //   bson_destroy(&reply);
  //   fail();
  // }
  // if (!mongoc_collection_insert_one(entries, doc2, NULL, &reply, &error)) {
  //   char *bson_as_json = bson_as_canonical_extended_json(doc2, NULL);
  //   t_log(ERROR, __func__,
  //         "failed to insert document: %s\n"
  //         "error code: [%d]\n"
  //         "error message: [%s]\n"
  //         "error reply: [%s]\n",
  //         bson_as_json, error.code, error.message,
  //         bson_as_canonical_extended_json(&reply, NULL));
  //   bson_free(bson_as_json);
  //   bson_destroy(&reply);
  //   fail();
  // }

  // // Then
  // bson_t *query = bson_new();
  // BSON_APPEND_UTF8(query, DB_KEY_ACTIVITY, s->TEST_ACTIVITY_1);
  // char *str;
  // mongoc_cursor_t *cursor =
  //     mongoc_collection_find_with_opts(entries, query, NULL, NULL);
  // const bson_t *result;
  // int result_count = 0;
  // while (mongoc_cursor_next(cursor, &result)) {
  //   result_count++;
  //   str = bson_as_canonical_extended_json(&*result, NULL);
  //   t_log(INFO, __func__, "Entry no: %d\n%s\n", result_count, str);
  //   bson_free(str);
  // }

  // assert_int_equal(result_count, 1);

  // // Finally
  // bson_destroy(doc1);
  // bson_destroy(doc2);
  // mongoc_cursor_destroy(cursor);
  // bson_destroy(query);
}

void test_db_get_by(void **state) {
  // // Given
  // test_state_t *s = (test_state_t *)*state;
  // mongoc_collection_drop(entries, NULL);

  // // When
  // bool sucess_1 = save(s->test_document_1);
  // bool sucess_2 = save(s->test_document_2);
  // bson_t_list *entries_by_project = get_by(DB_KEY_PROJECT, s->TEST_PROJECT);
  // bson_t_list *entries_by_activity = get_by(DB_KEY_ACTIVITY,
  // s->TEST_ACTIVITY_2); bson_t_list *entries_by_duration =
  //     get_by(DB_KEY_DURATION, &s->TEST_DURATION_S);

  // // Then
  // assert_true(sucess_1);
  // assert_true(sucess_2);
  // assert_non_null(entries_by_project->value);
  // assert_non_null(entries_by_activity->value);
  // _compare_entries(s->test_document_1, entries_by_project->value);
  // _compare_entries(s->test_document_2, entries_by_activity->value);
  // _compare_entries(s->test_document_1, entries_by_duration->value);
  // assert_int_equal(count_elements(entries_by_duration), 2);

  // // Finally
  // free_list(entries_by_project);
  // free_list(entries_by_activity);
  // free_list(entries_by_duration);
}
