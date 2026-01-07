#include "test_trackme.h"

#include <cmocka.h>
#ifdef stop
#undef stop
#endif // stop

#include "../lib/mongoose.h"
#include "../src/track_me.h"
#include "../src/util/log.h"
#include <bson/bson.h>
#include <cmocka.h>

/*** Internal helper test functions ***/

char **_get_start_info_activities(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->activity;
  ret[1] = si2->activity;
  return ret;
}

char **_get_start_info_clients(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->client;
  ret[1] = si2->client;
  return ret;
}

char **_get_start_info_projects(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->project;
  ret[1] = si2->project;
  return ret;
}

char **_get_start_info_descriptions(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->description;
  ret[1] = si2->description;
  return ret;
}

bool _compare_start_info(StartInfo *si1, StartInfo *si2) {
  char **activities = _get_start_info_activities(si1, si2);
  t_log(INFO, __func__, "Activities are [%s] and [%s]", activities[0],
        activities[1]);
  assert_string_equal(activities[0], activities[1]);
  free(activities);

  char **clients = _get_start_info_clients(si1, si2);
  t_log(INFO, __func__, "Clients are [%s] and [%s]", clients[0], clients[1]);
  assert_string_equal(clients[0], clients[1]);
  free(clients);

  char **projects = _get_start_info_projects(si1, si2);
  t_log(INFO, __func__, "Projects are [%s] and [%s]", projects[0], projects[1]);
  assert_string_equal(projects[0], projects[1]);
  free(projects);

  char **descriptions = _get_start_info_descriptions(si1, si2);
  t_log(INFO, __func__, "Descriptions are [%s] and [%s]", descriptions[0],
        descriptions[1]);
  assert_string_equal(descriptions[0], descriptions[1]);
  free(descriptions);

  return true;
}

bool _compare_update_info(UpdateInfo *ui1, UpdateInfo *ui2) {
  assert_true(_compare_start_info(ui1->info, ui2->info));
  assert_int_equal(ui1->end_time, ui2->end_time);
  assert_int_equal(ui1->start_time, ui2->start_time);

  return true;
}

/*** Tests begin ***/
/*** Internal functions ***/
void test_trackme_parse_start_info_request_body(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  StartInfo *si = _start_info_from_request_body(
      ERROR, s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // Then
  assert_non_null(si);
  assert_true(_compare_start_info(s->default_start_info, si));

  // Finally
  free_start_info(si);
}

void test_trackme_parse_update_info_request_body(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  UpdateInfo *ui =
      _update_info_from_request_body(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY);

  // Then
  assert_non_null(ui);
  assert_true(_compare_update_info(s->default_update_info, ui));

  // Finally
  free_update_info(ui);
}

void test_trackme_parse_start_info_NULL_request_body(void **state) {
  // Given

  // When
  StartInfo *fail_null_si = _start_info_from_request_body(ERROR, NULL);

  // Then
  assert_null(fail_null_si);

  // Finally
}

void test_trackme_parse_update_info_NULL_request_body(void **state) {
  // Given

  // When
  UpdateInfo *fail_null_ui = _update_info_from_request_body(NULL);

  // Then
  assert_null(fail_null_ui);

  // Finally
}

void test_trackme_parse_start_info_empty_request_body(void **state) {
  // Given
  struct mg_str invalid_body = mg_str("");

  // When
  StartInfo *fail_empty_si =
      _start_info_from_request_body(ERROR, &invalid_body);

  // Then
  assert_non_null(fail_empty_si);

  // Finally
  free_start_info(fail_empty_si);
}

void test_trackme_parse_update_info_empty_request_body(void **state) {
  // Given
  struct mg_str invalid_body = mg_str("");

  // When
  UpdateInfo *fail_empty_ui = _update_info_from_request_body(&invalid_body);

  // Then
  assert_non_null(fail_empty_ui);

  // Finally
  free_update_info(fail_empty_ui);
}

void test_trackme_time_t_to_string(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  char *start_time_str = _time_t_to_str(&s->TEST_START_TIME_S);

  // Then
  assert_non_null(start_time_str);
  assert_string_equal(start_time_str, EXPECTED_START_TIME);

  // Finally
  free(start_time_str);
}

void test_trackme_time_t_null_to_string(void **state) {
  // Given

  // When
  char *start_time_char = _time_t_to_str(NULL);

  // Then
  assert_non_null(start_time_char);
  assert_string_equal(start_time_char, NULL_TIME);

  // Finally
  free(start_time_char);
}

void test_trackme_get_duration_int_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  int duration_sec = _get_duration_int();

  // Then
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_int_equal(duration_sec, s->TEST_DURATION_S);

  // Finally
}

void test_trackme_get_duration_int_not_started(void **state) {
  // Given

  // When
  int duration_sec = _get_duration_int();

  // Then
  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_int_equal(duration_sec, -1);

  // Finally
}

void test_trackme_get_duration_int_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  int duration_sec = _get_duration_int();

  // Then
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());
  assert_int_equal(duration_sec, s->TEST_DURATION_S);

  // Finally
}
void test_trackme_duration_int_to_string(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  char *duration_str = _duration_int_to_string(s->TEST_DURATION_S);

  // Then
  assert_non_null(duration_str);
  assert_string_equal(duration_str, EXPECTED_DURATION);

  // Finally
  free(duration_str);
}

void test_trackme_duration_negative_int_to_string(void **state) {
  // Given

  // When
  char *duration_str = _duration_int_to_string(-1);

  // Then
  assert_null(duration_str);

  // Finally
  free(duration_str);
}

/* Public functions */

void test_trackme_start_timer_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  bool fail = start_timer(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY);

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_false(fail);
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(activity, s->default_start_info->activity);
  assert_string_equal(client, s->default_start_info->client);
  assert_string_equal(project, s->default_start_info->project);
  assert_string_equal(description, s->default_start_info->description);
  assert_string_equal(start, EXPECTED_START_TIME);
  assert_string_equal(end, NO_END_TIME);
  assert_string_equal(
      duration,
      EXPECTED_DURATION); // If timer was not restarted with correct start time
                          // this should fail as duration would be 0

  // Finally
  free(activity);
  free(start);
  free(description);
  free(project);
  free(client);
  free(end);
  free(duration);
}

void test_trackme_start_timer_not_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  bool succes = start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_true(succes);
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(activity, s->default_start_info->activity);
  assert_string_equal(client, s->default_start_info->client);
  assert_string_equal(project, s->default_start_info->project);
  assert_string_equal(description, s->default_start_info->description);
  assert_string_equal(start, EXPECTED_START_TIME);
  assert_string_equal(end, NO_END_TIME);
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(activity);
  free(client);
  free(project);
  free(description);
  free(start);
  free(end);
  free(duration);
}

void test_trackme_update_timer_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  bool updated = update_timer(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY);

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_true(updated);
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(activity, s->default_update_info->info->activity);
  assert_string_equal(client, s->default_update_info->info->client);
  assert_string_equal(project, s->default_update_info->info->project);
  assert_string_equal(description, s->default_update_info->info->description);
  assert_string_equal(start, EXPECTED_UPDATED_START_TIME);
  assert_string_equal(end, NO_END_TIME);
  assert_string_equal(duration, EXPECTED_UPDATED_DURATION);

  // Finally
  free(activity);
  free(client);
  free(project);
  free(description);
  free(start);
  free(end);
  free(duration);
};

void test_trackme_update_timer_not_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  bool updated = update_timer(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY);

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(updated);
  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_null(activity);
  assert_null(client);
  assert_null(project);
  assert_null(description);
  assert_null(start);
  assert_null(end);
  assert_null(duration);

  // Finally
};

void test_trackme_update_timer_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  bool updated = update_timer(s->TEST_UPDATE_INFO_HTTP_REQUEST_BODY);

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(updated);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());
  assert_string_equal(activity, s->default_start_info->activity);
  assert_string_equal(client, s->default_start_info->client);
  assert_string_equal(project, s->default_start_info->project);
  assert_string_equal(description, s->default_start_info->description);
  assert_string_equal(start, EXPECTED_START_TIME);
  assert_string_equal(end, EXPECTED_END_TIME);
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(activity);
  free(client);
  free(project);
  free(description);
  free(start);
  free(end);
  free(duration);
};

void test_trackme_reset_timer_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  reset_timer();

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(is_timer_running());
  assert_null(current_timer_result);
  assert_null(activity);
  assert_null(client);
  assert_null(project);
  assert_null(description);
  assert_null(start);
  assert_null(end);
  assert_null(duration);

  // Finally
};

void test_trackme_reset_timer_not_started(void **state) {
  // Given

  // When
  reset_timer();

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(is_timer_running());
  assert_null(current_timer_result);
  assert_null(activity);
  assert_null(client);
  assert_null(project);
  assert_null(description);
  assert_null(start);
  assert_null(end);
  assert_null(duration);

  // Finally
};

void test_trackme_reset_timer_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  reset_timer();

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(is_timer_running());
  assert_null(current_timer_result);
  assert_null(activity);
  assert_null(client);
  assert_null(project);
  assert_null(description);
  assert_null(start);
  assert_null(end);
  assert_null(duration);

  // Finally
};

void test_trackme_is_timer_running_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  bool running = is_timer_running();

  // Then
  assert_true(running);

  // Finally
}

void test_trackme_is_timer_running_not_started(void **state) {
  // Given
  // Not started means no call to time() to get end_time, thus no mock needed
  stop_timer();

  // When
  bool running = is_timer_running();

  // Then
  assert_false(running);

  // Finally
}

void test_trackme_is_timer_running_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  bool running = is_timer_running();

  // Then
  assert_false(running);

  // Finally
}

void test_trackme_stop_timer_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *expected_start = _time_t_to_str(&s->TEST_START_TIME_S);
  char *end = get_end_time();
  char *expected_end = _time_t_to_str(&s->TEST_END_TIME_S);
  char *duration = get_duration();
  char *expected_duration = _duration_int_to_string(s->TEST_DURATION_S);

  // Then
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());
  assert_string_equal(activity, s->default_start_info->activity);
  assert_string_equal(client, s->default_start_info->client);
  assert_string_equal(project, s->default_start_info->project);
  assert_string_equal(description, s->default_start_info->description);
  assert_string_equal(start, expected_start);
  assert_string_equal(end, expected_end);
  assert_string_equal(duration, expected_duration);

  // Finally
  free(activity);
  free(client);
  free(project);
  free(description);
  free(start);
  free(end);
  free(duration);
  free(expected_duration);
  free(expected_start);
  free(expected_end);
}

void test_trackme_stop_timer_not_started(void **state) {
  // Given

  // When
  bool success = stop_timer();

  char *activity = get_activity();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(success); // timer is stopped but no result is created; something
                         // is wrong (i.e. not started)!
  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_null(activity);
  assert_null(client);
  assert_null(project);
  assert_null(description);
  assert_null(start);
  assert_null(end);
  assert_null(duration);

  // Finally
}

void test_trackme_get_start_time_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  char *start_time = get_start_time();
  char *end_time = get_end_time();

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_string_equal(start_time, EXPECTED_START_TIME);

  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(end_time, NO_END_TIME);
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(start_time);
  free(end_time);
  free(duration);
}

void test_trackme_get_start_time_not_started(void **state) {
  // Given

  // When
  char *start_time = get_start_time();
  char *end_time = get_end_time();
  char *duration = get_duration();

  // Then
  assert_null(start_time);

  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_null(end_time);
  assert_null(duration);

  // Finally
}

void test_trackme_get_start_time_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *start_time = get_start_time();
  char *end_time = get_end_time();
  char *duration = get_duration();

  // Then
  assert_string_equal(start_time, EXPECTED_START_TIME);

  assert_non_null(current_timer_result);
  assert_false(is_timer_running());
  assert_string_equal(end_time, EXPECTED_END_TIME);
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(start_time);
  free(end_time);
  free(duration);
}

void test_trackme_get_end_time_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  char *end_time = get_end_time();

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_string_equal(end_time, NO_END_TIME);
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(end_time);
  free(duration);
}

void test_trackme_get_end_time_not_started(void **state) {
  // Given

  // When
  char *end_time = get_end_time();

  // Then
  assert_null(end_time);
  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
}

void test_trackme_get_end_time_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *end_time = get_end_time();
  char *duration = get_duration();

  // Then
  assert_string_equal(end_time, EXPECTED_END_TIME);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(end_time);
  free(duration);
}

void test_trackme_get_duration_not_started(void **state) {
  // Given

  // When
  char *duration = get_duration();

  // Then
  assert_null(duration);
  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
}

void test_trackme_get_duration_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_string_equal(duration, EXPECTED_DURATION);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(duration);
}

void test_trackme_get_duration_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *duration = get_duration();

  // Then
  assert_string_equal(duration, EXPECTED_DURATION);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(duration);
}

void test_trackme_get_activity_not_started(void **state) {
  // Given
  // When
  char *activity = get_activity();

  // Then
  assert_null(activity);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
}

void test_trackme_get_activity_started_not_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->NOT_SET_TEST_HTTP_REQUEST_BODY);

  // When
  char *activity = get_activity();

  // Then
  assert_string_equal(activity, NOT_SET);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(activity);
}

void test_trackme_get_activity_started_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  char *activity = get_activity();

  // Then
  assert_string_equal(activity, s->default_start_info->activity);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(activity);
}

void test_trackme_get_activity_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *activity = get_activity();

  // Then
  assert_string_equal(activity, s->default_start_info->activity);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(activity);
}

void test_trackme_get_client_not_started(void **state) {
  // Given
  // When
  char *client = get_client();

  // Then
  assert_null(client);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
}

void test_trackme_get_client_started_not_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->NOT_SET_TEST_HTTP_REQUEST_BODY);

  // When
  char *client = get_client();

  // Then
  assert_string_equal(client, NOT_SET);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(client);
}

void test_trackme_get_client_started_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  char *client = get_client();

  // Then
  assert_string_equal(client, s->default_start_info->client);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(client);
}

void test_trackme_get_client_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *client = get_client();

  // Then
  assert_string_equal(client, s->default_start_info->client);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(client);
}

void test_trackme_get_project_not_started(void **state) {
  // Given
  // When
  char *project = get_project();

  // Then
  assert_null(project);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
}

void test_trackme_get_project_started_not_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->NOT_SET_TEST_HTTP_REQUEST_BODY);

  // When
  char *project = get_project();

  // Then
  assert_string_equal(project, NOT_SET);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(project);
}

void test_trackme_get_project_started_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  char *project = get_project();

  // Then
  assert_string_equal(project, s->default_start_info->project);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(project);
}

void test_trackme_get_project_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *project = get_project();

  // Then
  assert_string_equal(project, s->default_start_info->project);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(project);
}

void test_trackme_get_description_not_started(void **state) {
  // Given
  // When
  char *description = get_description();

  // Then
  assert_null(description);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
}

void test_trackme_get_description_started_not_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->NOT_SET_TEST_HTTP_REQUEST_BODY);

  // When
  char *description = get_description();

  // Then
  assert_string_equal(description, NOT_SET);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(description);
}

void test_trackme_get_description_started_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  // When
  char *description = get_description();

  // Then
  assert_string_equal(description, s->default_start_info->description);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(description);
}

void test_trackme_get_description_stopped(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  CMockaValueData null_ptr_value_data = cast_ptr_to_cmocka_value(NULL);
  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_START_INFO_HTTP_REQUEST_BODY);

  expect_check_data(__wrap_time, __timer, check_pointer, null_ptr_value_data);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *description = get_description();

  // Then
  assert_string_equal(description, s->default_start_info->description);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(description);
}
