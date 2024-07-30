#include "integration_test.h"
#include "../src/db.h"
#include "test_db.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));

  const char *uri_string = "mongodb://blaklinten:test-psw@localhost:27017";
  init_db(uri_string);
  mongoc_collection_drop(entries, NULL);

  s->TEST_NAME_1 = "Lucas";
  s->TEST_NAME_2 = "Jonas";
  s->TEST_CLIENT = "test client";
  s->TEST_PROJECT = "test project";
  s->TEST_START_TIME_S = 1721664594;           // Mon 22 Jul 18:09:54 CEST 2024
  s->TEST_END_TIME_S = 1721669586;             // Mon 22 Jul 19:33:06 CEST 2024
  s->TEST_DURATION_S = 60 * 60 + 60 * 23 + 12; // 1h23min12s

  s->test_document_1 = bson_new();
  bson_oid_init(&s->test_id_1, NULL);
  BSON_APPEND_OID(s->test_document_1, DB_KEY_ID, &s->test_id_1);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_NAME, s->TEST_NAME_1);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_CLIENT, s->TEST_CLIENT);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_PROJECT, s->TEST_PROJECT);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_START_TIME,
                     s->TEST_START_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_END_TIME, s->TEST_END_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_DURATION, s->TEST_DURATION_S);

  s->test_document_2 = bson_new();
  bson_oid_init(&s->test_id_2, NULL);
  BSON_APPEND_OID(s->test_document_2, DB_KEY_ID, &s->test_id_2);
  BSON_APPEND_UTF8(s->test_document_2, DB_KEY_NAME, s->TEST_NAME_2);
  BSON_APPEND_UTF8(s->test_document_2, DB_KEY_CLIENT, s->TEST_CLIENT);
  BSON_APPEND_UTF8(s->test_document_2, DB_KEY_PROJECT, s->TEST_PROJECT);
  BSON_APPEND_TIME_T(s->test_document_2, DB_KEY_START_TIME,
                     s->TEST_START_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_2, DB_KEY_END_TIME, s->TEST_END_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_2, DB_KEY_DURATION, s->TEST_DURATION_S);

  *state = s;

  return 0;
}

static int group_teardown(void **state) {
  test_state_t *s = (test_state_t *)*state;

  if (s) {
    if (s->test_document_1) {
      bson_destroy(s->test_document_1);
    }
    if (s->test_document_2) {
      bson_destroy(s->test_document_2);
    }
    free(s);
  }

  free_db();

  return 0;
}

int main(void) {

  const struct CMUnitTest integration_test[] = {
      cmocka_unit_test(test_db_connect),
      cmocka_unit_test(test_db_insert_and_get),
      cmocka_unit_test(test_db_save),
      cmocka_unit_test(test_db_get_by),
  };

  return cmocka_run_group_tests(integration_test, group_setup, group_teardown);
}
