#include "unit_test.h"
#include "../src/db.h"
#include "../src/timer.h"
#include "../src/track_me.h"
#include "test_list.h"
#include "test_timer.h"
#include "test_trackme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mock time(time_t * __timer)
time_t __wrap_time(time_t *__timer) {
  check_expected_ptr(__timer);

  time_t mock_time = mock_type(time_t);

  if (__timer) {
    *__timer = mock_time;
  }

  return mock_time;
}

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));
  // s->TEST_START_TIME_S = 1690876956; // Tue  1 Aug 10:02:36 CEST 2023
  // s->TEST_END_TIME_S = 1690879255;   // Tue  1 Aug 10:40:55 CEST 2023
  StartInfo *si = malloc(sizeof(StartInfo));
  si->activity = "test_activity_name";
  si->client = "test_client";
  si->project = "test_project";
  si->description = "this is a test description";
  s->default_start_info = si;

  StartInfo *info = malloc(sizeof(StartInfo));
  info->activity = "updated_activity";
  info->client = "updated_client";
  info->project = "updated_project";
  info->description = "this is an updated description";
  UpdateInfo *ui = malloc(sizeof(UpdateInfo));
  ui->info = info,
  ui->start_time = 1721665160; // Mon 22 Jul 18:19:20 CEST 2024
  ui->end_time = 1721668890;   // Mon 22 Jul 19:20:30 CEST 2024
  s->default_update_info = ui;

  // TODO Clean this up! Do we need duplicate test_activity_name - TEST_ACTIVITY
  // and so on...?
  s->TEST_ACTIVITY_1 = "Alice";
  s->TEST_ACTIVITY_2 = "Bob";
  s->TEST_CLIENT = "test client";
  s->TEST_PROJECT = "test project";
  s->TEST_START_TIME_S = 1721664594;           // Mon 22 Jul 18:09:54 CEST 2024
  s->TEST_END_TIME_S = 1721669586;             // Mon 22 Jul 19:33:06 CEST 2024
  s->TEST_DURATION_S = 60 * 60 + 60 * 23 + 12; // 1h23min12s

  s->test_document_1 = bson_new();
  bson_oid_init(&s->test_id_1, NULL);
  BSON_APPEND_OID(s->test_document_1, DB_KEY_ID, &s->test_id_1);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_ACTIVITY, s->TEST_ACTIVITY_1);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_CLIENT, s->TEST_CLIENT);
  BSON_APPEND_UTF8(s->test_document_1, DB_KEY_PROJECT, s->TEST_PROJECT);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_START_TIME, s->TEST_START_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_END_TIME, s->TEST_END_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_1, DB_KEY_DURATION, s->TEST_DURATION_S);

  s->test_document_2 = bson_new();
  bson_oid_init(&s->test_id_2, NULL);
  BSON_APPEND_OID(s->test_document_2, DB_KEY_ID, &s->test_id_2);
  BSON_APPEND_UTF8(s->test_document_2, DB_KEY_ACTIVITY, s->TEST_ACTIVITY_2);
  BSON_APPEND_UTF8(s->test_document_2, DB_KEY_CLIENT, s->TEST_CLIENT);
  BSON_APPEND_UTF8(s->test_document_2, DB_KEY_PROJECT, s->TEST_PROJECT);
  BSON_APPEND_TIME_T(s->test_document_2, DB_KEY_START_TIME, s->TEST_START_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_2, DB_KEY_END_TIME, s->TEST_END_TIME_S);
  BSON_APPEND_TIME_T(s->test_document_2, DB_KEY_DURATION, s->TEST_DURATION_S);

  char *start_info_buf = malloc(5 * REQUEST_FIELD_SHORT_SIZE);
  snprintf(start_info_buf, 5 * REQUEST_FIELD_SHORT_SIZE ,
           "client=%s&project=%s&activity=%s&description=%s",
           s->default_start_info->client, s->default_start_info->project,
           s->default_start_info->activity, s->default_start_info->description);
  struct mg_str *start_body = malloc(sizeof(struct mg_str));
  *start_body = mg_str(start_info_buf);
  s->TEST_START_INFO_HTTP_REQUEST_BODY = start_body;

  char *update_info_buf = malloc(5 * REQUEST_FIELD_SHORT_SIZE + 2 * sizeof(time_t));
  snprintf(update_info_buf, 5 * REQUEST_FIELD_SHORT_SIZE + 2 * sizeof(time_t),
           "client=%s&project=%s&activity=%s&description=%s&start_time=%ld&"
           "end_time=%ld",
           s->default_update_info->info->client, s->default_update_info->info->project,
           s->default_update_info->info->activity, s->default_update_info->info->description,
           s->default_update_info->start_time, s->default_update_info->end_time);
  struct mg_str *update_body = malloc(sizeof(struct mg_str));
  *update_body = mg_str(update_info_buf);
  s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY = update_body;

  char *not_set_buf = malloc(5 * REQUEST_FIELD_SHORT_SIZE);
  snprintf(not_set_buf, 5 * REQUEST_FIELD_SHORT_SIZE,
           "client=%s&project=%s&activity=%s&description=%s", "", "", "", "");
  struct mg_str *not_set_body = malloc(sizeof(struct mg_str));
  *not_set_body = mg_str(not_set_buf);
  s->NOT_SET_TEST_HTTP_REQUEST_BODY = not_set_body;
  *state = s;

  return 0;
}

static int group_teardown(void **state) {
  test_state_t *s = (test_state_t *)*state;

  if (s) {
    if (s->TEST_START_INFO_HTTP_REQUEST_BODY) {
      free(s->TEST_START_INFO_HTTP_REQUEST_BODY->buf);
      free(s->TEST_START_INFO_HTTP_REQUEST_BODY);
    }
    if (s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY) {
      free(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY->buf);
      free(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY);
    }
    if (s->NOT_SET_TEST_HTTP_REQUEST_BODY) {
      free(s->NOT_SET_TEST_HTTP_REQUEST_BODY->buf);
      free(s->NOT_SET_TEST_HTTP_REQUEST_BODY);
    }
    if (s->default_start_info) {
      free(s->default_start_info);
    }
    if (s->default_update_info) {
      free(s->default_update_info->info);
      free(s->default_update_info);
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

int check_pointer (CMockaValueData value, CMockaValueData expected) {
  return value.ptr == expected.ptr;
}

static int reset_timer(void **state) {
  test_state_t *s = (test_state_t *)*state;
  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, 1);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, 1);
  stop_timer();

  free_timer_result(current_timer_result);
  current_timer_result = NULL;
  return 0;
}

int main(void) {

  const struct CMUnitTest unit_test[] = {
      /* Timer */
      cmocka_unit_test(test_timer_reset),
      cmocka_unit_test(test_timer_free_timer_result),
      cmocka_unit_test(test_timer_free_start_info),
      cmocka_unit_test(test_timer_free_update_info),
      cmocka_unit_test(test_timer_start),
      cmocka_unit_test(test_timer_start_NULL),
      cmocka_unit_test(test_timer_start_NULL_start_info),
      cmocka_unit_test(test_timer_stop_started),
      cmocka_unit_test(test_timer_stop_NULL),
      cmocka_unit_test(test_timer_stop_not_started),
      cmocka_unit_test(test_timer_update),
      cmocka_unit_test(test_timer_update_NULL),
      cmocka_unit_test(test_timer_update_NULL_update_info),
      cmocka_unit_test(test_timer_update_not_started),
      cmocka_unit_test(test_timer_get_activity),
      cmocka_unit_test(test_timer_not_started_get_activity),
      cmocka_unit_test(test_timer_get_client),
      cmocka_unit_test(test_timer_not_started_get_client),
      cmocka_unit_test(test_timer_get_project),
      cmocka_unit_test(test_timer_not_started_get_project),
      cmocka_unit_test(test_timer_get_description),
      cmocka_unit_test(test_timer_not_started_get_description),
      /* List */
      cmocka_unit_test(test_list_empty_create),
      cmocka_unit_test(test_list_create_from_NULL),
      cmocka_unit_test(test_list_create_from_document),
      cmocka_unit_test(test_list_add_element),
      cmocka_unit_test(test_list_add_invalid_element),
      cmocka_unit_test(test_list_free),
      cmocka_unit_test(test_list_count_element),
      /* Trackme */
      /* private functions */
      cmocka_unit_test_teardown(test_trackme_parse_start_info_request_body, reset_timer),
      cmocka_unit_test_teardown(test_trackme_parse_update_info_request_body, reset_timer),
      cmocka_unit_test_teardown(test_trackme_parse_start_info_NULL_request_body, reset_timer),
      cmocka_unit_test_teardown(test_trackme_parse_update_info_NULL_request_body, reset_timer),
      cmocka_unit_test_teardown(test_trackme_parse_start_info_empty_request_body, reset_timer),
      cmocka_unit_test_teardown(test_trackme_parse_update_info_empty_request_body, reset_timer),
      cmocka_unit_test_teardown(test_trackme_time_t_to_string, reset_timer),
      cmocka_unit_test_teardown(test_trackme_time_t_null_to_string, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_duration_int_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_duration_int_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_duration_int_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_duration_int_to_string, reset_timer),
      cmocka_unit_test_teardown(test_trackme_duration_negative_int_to_string, reset_timer),
      /* public functions */
      cmocka_unit_test_teardown(test_trackme_start_timer_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_start_timer_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_is_timer_running_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_is_timer_running_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_is_timer_running_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_stop_timer_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_stop_timer_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_start_time_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_start_time_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_start_time_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_end_time_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_end_time_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_end_time_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_duration_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_duration_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_duration_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_activity_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_activity_started_not_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_activity_started_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_activity_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_client_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_client_started_not_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_client_started_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_client_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_project_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_project_started_not_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_project_started_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_project_stopped, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_description_not_started, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_description_started_not_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_description_started_set, reset_timer),
      cmocka_unit_test_teardown(test_trackme_get_description_stopped, reset_timer),
  };

  return cmocka_run_group_tests(unit_test, group_setup, group_teardown);
}
