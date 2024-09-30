#include "test_trackme.h"
#include "../src/track_me.h"
#include "../src/util/log.h"
#include "unit_test.h"
#include <bson/bson.h>
#include <cmocka.h>

char expected[] = "01:23:12";

void test_trackme_get_current_duration_str(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  expect_value(__wrap_time, __timer, NULL);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);

  // When
  char *duration = get_current_duration_str(&t);

  // Then
  assert_non_null(duration);
  assert_string_equal(expected, duration);

  // Finally
  free(duration);
  reset(&t);
}

void test_trackme_get_current_duration_str_not_started(void **state) {
  // Given
  char zero_duration_string[] = "00:00:00";
  Timer t;
  reset(&t);

  // When
  char *fail_not_started = get_current_duration_str(&t);

  // Then
  assert_non_null(fail_not_started);
  assert_string_equal(zero_duration_string, fail_not_started);

  // Finally
  free(fail_not_started);
  reset(&t);
}

void test_trackme_get_current_duration_str_NULL(void **state) {
  // Given
  char zero_duration_string[] = "00:00:00";
  Timer t;
  reset(&t);

  // When
  char *fail_null = get_current_duration_str(NULL);

  // Then
  assert_non_null(fail_null);
  assert_string_equal(zero_duration_string, fail_null);

  // Finally
  free(fail_null);
  reset(&t);
}

void test_trackme_get_final_duration_str(void **state) {
  // Given
  char no_timer_result_str[] = "No TimerResult";
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  expect_value(__wrap_time, __timer, NULL);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  TimerResult *result = stop(&t);

  // When
  char *duration = get_final_duration_str(result);
  char *fail_null = get_final_duration_str(NULL);

  // Then
  assert_non_null(fail_null);
  assert_string_equal(no_timer_result_str, fail_null);
  assert_non_null(duration);
  assert_string_equal(expected, duration);
  // Finally
  free(duration);
  free(fail_null);
  reset(&t);
  free_timer_result(result);
}

void test_trackme_get_final_duration_str_not_started(void **state) {
  // Given
  char no_timer_result_str[] = "No TimerResult";
  Timer t;
  reset(&t);
  TimerResult *result = stop(&t);

  // When
  char *duration = get_final_duration_str(result);

  // Then
  assert_non_null(duration);
  assert_string_equal(no_timer_result_str, duration);

  // Finally
  free(duration);
  reset(&t);
  free_timer_result(result);
}

void test_trackme_get_final_duration_str_NULL(void **state) {
  // Given
  char no_timer_result_str[] = "No TimerResult";
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  expect_value(__wrap_time, __timer, NULL);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  TimerResult *result = stop(&t);

  // When
  char *duration = get_final_duration_str(result);
  char *fail_null = get_final_duration_str(NULL);

  // Then
  assert_non_null(fail_null);
  assert_string_equal(no_timer_result_str, fail_null);
  assert_non_null(duration);
  assert_string_equal(expected, duration);
  // Finally
  free(duration);
  free(fail_null);
  reset(&t);
  free_timer_result(result);
}

void test_trackme_get_start_time_str(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  expect_value(__wrap_time, __timer, NULL);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  TimerResult *result = stop(&t);
  char expected[] = "Mon, 22 Jul 2024 18:09:54 +0200";

  // When
  char *start_time = get_start_time_str(result);
  char *fail_null = get_start_time_str(NULL);

  // Then
  assert_non_null(fail_null);
  assert_string_equal("No TimerResult", fail_null);
  assert_non_null(start_time);
  assert_string_equal(expected, start_time);
  // Finally
  free(start_time);
  free(fail_null);
  reset(&t);
  free_timer_result(result);
}
void test_trackme_get_stop_time_str(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  expect_value(__wrap_time, __timer, NULL);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  TimerResult *result = stop(&t);
  char expected[] = "Mon, 22 Jul 2024 19:33:06 +0200";

  // When
  char *stop_time = get_stop_time_str(result);
  char *fail_null = get_stop_time_str(NULL);

  // Then
  assert_non_null(fail_null);
  assert_string_equal("No TimerResult", fail_null);
  assert_non_null(stop_time);
  assert_string_equal(expected, stop_time);
  // Finally
  free(stop_time);
  free(fail_null);
  reset(&t);
  free_timer_result(result);
}

char **get_start_info_names(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->name;
  ret[1] = si2->name;
  return ret;
}

char **get_start_info_clients(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->client;
  ret[1] = si2->client;
  return ret;
}

char **get_start_info_projects(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->project;
  ret[1] = si2->project;
  return ret;
}

char **get_start_info_descriptions(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->description;
  ret[1] = si2->description;
  return ret;
}

bool compare_start_info(StartInfo *si1, StartInfo *si2) {
  // Name
  char **names = get_start_info_names(si1, si2);
  t_log(INFO, __func__, "Names are [%s] and [%s]", names[0], names[1]);
  assert_string_equal(names[0], names[1]);
  free(names);

  // Client
  char **clients = get_start_info_clients(si1, si2);
  t_log(INFO, __func__, "Clients are [%s] and [%s]", clients[0], clients[1]);
  assert_string_equal(clients[0], clients[1]);
  free(clients);

  // Project
  char **projects = get_start_info_projects(si1, si2);
  t_log(INFO, __func__, "Projects are [%s] and [%s]", projects[0], projects[1]);
  assert_string_equal(projects[0], projects[1]);
  free(projects);

  // Start time
  char **descriptions = get_start_info_descriptions(si1, si2);
  t_log(INFO, __func__, "Descriptions are [%s] and [%s]", descriptions[0],
        descriptions[1]);
  assert_string_equal(descriptions[0], descriptions[1]);
  free(descriptions);

  return true;
}
void test_trackme_from_request_body(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  StartInfo *si = from_request_body(s->TEST_HTTP_REQUEST_BODY);

  // Then
  assert_non_null(si);
  assert_true(compare_start_info(s->default_test_info, si));

  // Finally
  free_start_info(si);
}
void test_trackme_from_NULL_request_body(void **state) {
  // Given
  // When
  StartInfo *fail_null = from_request_body(NULL);
  // Then
  assert_null(fail_null);
  // Finally
  free_start_info(fail_null);
}

void test_trackme_from_empty_request_body(void **state) {
  // Given
  struct mg_str invalid_body = mg_str("");
  // When
  StartInfo *fail_empty = from_request_body(&invalid_body);
  // Then
  assert_non_null(fail_empty);
  // Finally
  free_start_info(fail_empty);
}
