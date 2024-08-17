#include "unit_test.h"
#include "../src/db.h"
#include "test_timer.h"
#include "test_list.h"
#include <stdlib.h>

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));
  // s->TEST_START_TIME_S = 1690876956; // Tue  1 Aug 10:02:36 CEST 2023
  // s->TEST_END_TIME_S = 1690879255;   // Tue  1 Aug 10:40:55 CEST 2023
  StartInfo *si = malloc(sizeof(StartInfo));
  si->name = "test_name";
  si->client = "test_client";
  si->project = "test_project";
  si->description = "this is a test description";
  s->default_test_info = si;
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
    if (s->default_test_info) {
      free(s->default_test_info);
    }
    if (s->test_document_1) {
      bson_destroy(s->test_document_1);
    }
    if (s->test_document_2) {
      bson_destroy(s->test_document_2);
    }
    free(s);
  }

  return 0;
}

static int restore_test_document(void **state) {
  test_state_t *s = (test_state_t *)*state;

  s->test_document_1 = bson_new();
  BSON_APPEND_OID(s->test_document_1, DB_KEY_ID, &s->test_id_1);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_NAME, s->TEST_NAME_1);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_CLIENT, s->TEST_CLIENT);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_PROJECT, s->TEST_PROJECT);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_START_TIME,
                     s->TEST_START_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_END_TIME, s->TEST_END_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_DURATION, s->TEST_DURATION_S);

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
      cmocka_unit_test_setup_teardown(test_list_create, NULL,
                                      restore_test_document),
      cmocka_unit_test_setup_teardown(test_list_add_element, NULL,
                                      restore_test_document),
      cmocka_unit_test(test_list_count_element),
  };

  return cmocka_run_group_tests(unit_test, group_setup, group_teardown);
}
