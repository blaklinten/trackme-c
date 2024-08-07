#include "unit_test.h"

#include "../src/timer.h"
#include <stdlib.h>

// Mock time(time_t * __timer)
time_t __wrap_time(time_t *__timer) {
  check_expected_ptr(__timer);

  time_t mock_time = mock_type(time_t);

  if (__timer) {
    *__timer = mock_time;
  }

  return mock_time;
}

// Timer
void test_timer_reset() {
  // Given
  Timer t;
  // When
  reset(&t);

  // Then
  assert_null(t.name);
  assert_null(t.client);
  assert_null(t.project);
  assert_null(t.description);
  assert_int_equal(-1, t.start_time);
}

void test_timer_start(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  start(&t, s->default_test_info);

  // Then
  assert_ptr_not_equal(s->default_test_info->name, t.name);
  assert_string_equal(s->default_test_info->name, t.name);

  assert_ptr_not_equal(s->default_test_info->client, t.client);
  assert_string_equal(s->default_test_info->client, t.client);

  assert_ptr_not_equal(s->default_test_info->project, t.project);
  assert_string_equal(s->default_test_info->project, t.project);

  assert_ptr_not_equal(s->default_test_info->description, t.description);
  assert_string_equal(s->default_test_info->description, t.description);

  assert_true(t.start_time > 0);
  assert_true(t.start_time == s->TEST_START_TIME_S);

  reset(&t);
}

void test_timer_stop(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, s->default_test_info);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S); // diff = 38m 19s

  // When
  TimerResult *tr = stop(&t);

  // Then
  assert_ptr_not_equal(s->default_test_info->name, tr->name);
  assert_ptr_not_equal(s->default_test_info->client, tr->client);
  assert_ptr_not_equal(s->default_test_info->project, tr->project);
  assert_ptr_not_equal(s->default_test_info->description, tr->description);

  assert_string_equal(s->default_test_info->name, tr->name);
  assert_string_equal(s->default_test_info->client, tr->client);
  assert_string_equal(s->default_test_info->project, tr->project);
  assert_string_equal(s->default_test_info->description, tr->description);

  assert_int_equal(tr->start_time, s->TEST_START_TIME_S);
  assert_int_equal(tr->end_time, s->TEST_END_TIME_S);

  assert_non_null(tr->duration);
  assert_int_equal(tr->duration, s->TEST_END_TIME_S - s->TEST_START_TIME_S);

  free_timer_result(tr);
  reset(&t);
}

void test_timer_get_name(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, s->default_test_info);

  // When
  char *name = get_name(&t);

  // Then
  assert_string_equal(s->default_test_info->name, name);

  reset(&t);
  free(name);
}

void test_timer_get_client(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, s->default_test_info);

  // When
  char *client = get_client(&t);

  // Then
  assert_string_equal(s->default_test_info->client, client);

  reset(&t);
  free(client);
}

void test_timer_get_project(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, s->default_test_info);

  // When
  char *project = get_project(&t);

  // Then
  assert_string_equal(s->default_test_info->project, project);

  reset(&t);
  free(project);
}

void test_timer_get_description(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, s->default_test_info);

  // When
  char *description = get_description(&t);

  // Then
  assert_string_equal(s->default_test_info->description, description);

  reset(&t);
  free(description);
}

void test_timer_get_duration(void **state) {
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
  int duration = get_duration(&t);

  // Then
  assert_int_not_equal(0, duration);
  assert_int_equal(duration,
                   s->TEST_END_TIME_S -
                       s->TEST_START_TIME_S); // duration = 38m 19s

  reset(&t);
}
