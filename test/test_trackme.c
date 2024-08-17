#include "test_trackme.h"
#include "../src/track_me.h"
#include "../src/util/log.h"
#include "unit_test.h"
#include <bson/bson.h>
#include <cmocka.h>

void test_get_current_duration_str(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  expect_value(__wrap_time, __timer, NULL);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char expected[] = "01:23:12";

  // When
  char *duration = get_current_duration_str(&t);

  // Then
  assert_string_equal(expected, duration);
  // Finally
  free(duration);
  reset(&t);
}
void test_get_final_duration_str(void **state) {
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
  char expected[] = "01:23:12";

  // When
  char *duration = get_final_duration_str(result);

  // Then
  assert_string_equal(expected, duration);
  // Finally
  free(duration);
  reset(&t);
  free_timer_result(result);
}

void test_get_start_time_str(void **state) {
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

  // Then
  assert_string_equal(expected, start_time);
  // Finally
  free(start_time);
  reset(&t);
  free_timer_result(result);
}
void test_get_stop_time_str(void **state) {
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

  // Then
  assert_string_equal(expected, stop_time);
  // Finally
  free(stop_time);
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
void test_from_request_body(void **state) {
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
