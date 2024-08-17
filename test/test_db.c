#include "test_db.h"
#include "../src/db.h"
#include "../src/util/bson_list.h"
#include "../src/util/log.h"
#include "integration_test.h"

char **get_oids_as_strings(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_ID) ||
      !bson_iter_find(&e2_iter, DB_KEY_ID)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_ID);
    fail();
  }
  const bson_oid_t *oid1 = bson_iter_oid(&e1_iter);
  const bson_oid_t *oid2 = bson_iter_oid(&e2_iter);
  char *oid_string1 = malloc(25 * sizeof(char));
  char *oid_string2 = malloc(25 * sizeof(char));
  bson_oid_to_string(oid1, oid_string1);
  bson_oid_to_string(oid2, oid_string2);

  char **ret = malloc(2 * sizeof(char *));
  ret[0] = oid_string1;
  ret[1] = oid_string2;
  return ret;
}
char **get_bson_names(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_NAME) ||
      !bson_iter_find(&e2_iter, DB_KEY_NAME)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_NAME);
    fail();
  }
  const char *name1 = bson_iter_utf8(&e1_iter, NULL);
  const char *name2 = bson_iter_utf8(&e2_iter, NULL);
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = bson_strndup(name1, strlen(name1));
  ret[1] = bson_strndup(name2, strlen(name2));
  return ret;
}
char **get_bson_clients(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_CLIENT) ||
      !bson_iter_find(&e2_iter, DB_KEY_CLIENT)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_CLIENT);
    fail();
  }
  const char *client1 = bson_iter_utf8(&e1_iter, NULL);
  const char *client2 = bson_iter_utf8(&e2_iter, NULL);
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = bson_strndup(client1, strlen(client1));
  ret[1] = bson_strndup(client2, strlen(client2));
  return ret;
}

char **get_bson_projects(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_PROJECT) ||
      !bson_iter_find(&e2_iter, DB_KEY_PROJECT)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_PROJECT);
    fail();
  }
  const char *project1 = bson_iter_utf8(&e1_iter, NULL);
  const char *project2 = bson_iter_utf8(&e2_iter, NULL);
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = bson_strndup(project1, strlen(project1));
  ret[1] = bson_strndup(project2, strlen(project2));
  return ret;
}

time_t *get_bson_durations(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_DURATION) ||
      !bson_iter_find(&e2_iter, DB_KEY_DURATION)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_DURATION);
    fail();
  }

  time_t *ret = malloc(2 * sizeof(time_t *));
  ret[0] = bson_iter_time_t(&e1_iter);
  ret[1] = bson_iter_time_t(&e2_iter);
  return ret;
}

time_t *get_bson_start_times(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_START_TIME) ||
      !bson_iter_find(&e2_iter, DB_KEY_START_TIME)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_START_TIME);
    fail();
  }
  time_t *ret = malloc(2 * sizeof(time_t *));
  ret[0] = bson_iter_time_t(&e1_iter);
  ret[1] = bson_iter_time_t(&e2_iter);
  return ret;
}

time_t *get_bson_end_times(bson_t *e1, bson_t *e2) {
  bson_iter_t e1_iter, e2_iter;
  if (!bson_iter_init(&e1_iter, e1) || !bson_iter_init(&e2_iter, e2)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  if (!bson_iter_find(&e1_iter, DB_KEY_END_TIME) ||
      !bson_iter_find(&e2_iter, DB_KEY_END_TIME)) {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_END_TIME);
    fail();
  }
  time_t *ret = malloc(2 * sizeof(time_t *));
  ret[0] = bson_iter_time_t(&e1_iter);
  ret[1] = bson_iter_time_t(&e2_iter);
  return ret;
}

bool compare_entries(bson_t *e1, bson_t *e2) {

  // OID
  char **oids = get_oids_as_strings(e1, e2);
  assert_string_equal(oids[0], oids[1]);
  free(oids[0]);
  free(oids[1]);
  free(oids);

  // Name
  char **names = get_bson_names(e1, e2);
  assert_string_equal(names[0], names[1]);
  bson_free(names[0]);
  bson_free(names[1]);
  free(names);

  // Client
  char **clients = get_bson_clients(e1, e2);
  assert_string_equal(clients[0], clients[1]);
  bson_free(clients[0]);
  bson_free(clients[1]);
  free(clients);

  // Project
  char **projects = get_bson_projects(e1, e2);
  assert_string_equal(projects[0], projects[1]);
  bson_free(projects[0]);
  bson_free(projects[1]);
  free(projects);

  // Duraion
  time_t *durations = get_bson_durations(e1, e2);
  assert_false(difftime(durations[0], durations[1]));
  free(durations);

  // Start time
  time_t *start_times = get_bson_start_times(e1, e2);
  assert_false(difftime(start_times[0], start_times[1]));
  free(start_times);

  // End time
  time_t *end_times = get_bson_end_times(e1, e2);
  assert_false(difftime(end_times[0], end_times[1]));
  free(end_times);

  return true;
}

void test_db_connect() {
  // Given
  bson_t *command = NULL, reply;
  bson_error_t error;
  bool sucess = false;
  command = BCON_NEW("ping", BCON_INT32(1));

  // When
  sucess = mongoc_client_command_simple(db_client, "admin", command, NULL,
                                        &reply, &error);
  // Then
  if (!sucess) {
    t_log(ERROR, __func__, "Failed to ping db");
    fail();
  }

  // Finally
  bson_destroy(&reply);
  bson_destroy(command);
}

void test_db_save(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  bool sucess = save(s->test_document_1);
  bson_t_list *entry = get_by(DB_KEY_ID, &s->test_id_1);

  // Then
  assert_null(entry->next);
  bson_iter_t result_iter;
  if (!bson_iter_init(&result_iter, entry->value)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }

  const char *name;
  if (bson_iter_find(&result_iter, DB_KEY_NAME)) {
    name = bson_iter_utf8(&result_iter, NULL);
  } else {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_NAME);
    fail();
  }

  if (!bson_iter_init(&result_iter, entry->value)) {
    t_log(ERROR, __func__, "Could not init iterator");
    fail();
  }
  const bson_oid_t *oid = NULL;
  if (bson_iter_find(&result_iter, DB_KEY_ID)) {
    oid = bson_iter_oid(&result_iter);
    char oid_string[25];
    bson_oid_to_string(oid, oid_string);
  } else {
    t_log(ERROR, __func__, "No %s found in document", DB_KEY_ID);
    fail();
  }
  //
  // Finally
  assert_true(sucess);
  assert_true(bson_oid_equal(oid, &s->test_id_1));
  assert_string_equal(name, s->TEST_NAME_1);

  free_list(entry);
}

void test_db_insert_and_get(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  bson_t *doc1 = bson_new();
  BSON_APPEND_UTF8(doc1, DB_KEY_NAME, s->TEST_NAME_1);

  bson_t *doc2 = bson_new();
  BSON_APPEND_UTF8(doc2, DB_KEY_NAME, s->TEST_NAME_2);

  if (!entries) {
    t_log(ERROR, __func__, "No collection");
    fail();
  }

  bson_t reply;
  bson_error_t error;
  // When
  if (!mongoc_collection_insert_one(entries, doc1, NULL, &reply, &error)) {
    char *bson_as_json = bson_as_canonical_extended_json(doc1, NULL);
    t_log(ERROR, __func__,
          "failed to insert document: %s\n"
          "error code: [%d]\n"
          "error message: [%s]\n"
          "error reply: [%s]\n",
          bson_as_json, error.code, error.message,
          bson_as_canonical_extended_json(&reply, NULL));
    bson_free(bson_as_json);
    bson_destroy(&reply);
    fail();
  }
  if (!mongoc_collection_insert_one(entries, doc2, NULL, &reply, &error)) {
    char *bson_as_json = bson_as_canonical_extended_json(doc2, NULL);
    t_log(ERROR, __func__,
          "failed to insert document: %s\n"
          "error code: [%d]\n"
          "error message: [%s]\n"
          "error reply: [%s]\n",
          bson_as_json, error.code, error.message,
          bson_as_canonical_extended_json(&reply, NULL));
    bson_free(bson_as_json);
    bson_destroy(&reply);
    fail();
  }
  bson_destroy(doc1);
  bson_destroy(doc2);

  // Then
  bson_t *query = bson_new();
  BSON_APPEND_UTF8(query, DB_KEY_NAME, s->TEST_NAME_1);
  char *str;
  mongoc_cursor_t *cursor =
      mongoc_collection_find_with_opts(entries, query, NULL, NULL);
  const bson_t *result;
  int result_count = 0;
  while (mongoc_cursor_next(cursor, &result)) {
    result_count++;
    str = bson_as_canonical_extended_json(&*result, NULL);
    t_log(INFO, __func__, "Entry no: %d\n%s\n", result_count, str);
    bson_free(str);
  }
  // Finally
  mongoc_cursor_destroy(cursor);
  bson_destroy(query);
}

void test_db_get_by(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  mongoc_collection_drop(entries, NULL);

  // When
  bool sucess_1 = save(s->test_document_1);
  bool sucess_2 = save(s->test_document_2);
  bson_t_list *entries_by_project = get_by(DB_KEY_PROJECT, s->TEST_PROJECT);
  bson_t_list *entries_by_name = get_by(DB_KEY_NAME, s->TEST_NAME_2);
  bson_t_list *entries_by_duration =
      get_by(DB_KEY_DURATION, &s->TEST_DURATION_S);

  // Then
  assert_true(sucess_1);
  assert_true(sucess_2);
  assert_non_null(entries_by_project->value);
  assert_non_null(entries_by_name->value);
  compare_entries(s->test_document_1, entries_by_project->value);
  compare_entries(s->test_document_2, entries_by_name->value);
  compare_entries(s->test_document_1, entries_by_duration->value);
  assert_int_equal(2, count_elements(entries_by_duration));
  // Finally
  free_list(entries_by_project);
  free_list(entries_by_name);
  free_list(entries_by_duration);
}
