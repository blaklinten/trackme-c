#include "unit_test.h"

#include "../src/timer.h"
#include <cmocka.h>
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
  bool success = start(&t, s->default_test_info);

  // Then
  assert_true(success);

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

  // Finally
  reset(&t);
}

void test_timer_start_NULL(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  bool fail_null_timer = start(NULL, s->default_test_info);

  // Then
  assert_false(fail_null_timer);

  // Finally
}
void test_timer_start_NULL_start_info(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  bool fail_null_start_info = start(&t, NULL);

  // Then
  assert_false(fail_null_start_info);

  // Finally
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
  TimerResult *sucess = stop(&t);

  // Then
  assert_non_null(sucess);

  assert_ptr_not_equal(s->default_test_info->name, sucess->name);
  assert_ptr_not_equal(s->default_test_info->client, sucess->client);
  assert_ptr_not_equal(s->default_test_info->project, sucess->project);
  assert_ptr_not_equal(s->default_test_info->description, sucess->description);

  assert_string_equal(s->default_test_info->name, sucess->name);
  assert_string_equal(s->default_test_info->client, sucess->client);
  assert_string_equal(s->default_test_info->project, sucess->project);
  assert_string_equal(s->default_test_info->description, sucess->description);

  assert_int_equal(sucess->start_time, s->TEST_START_TIME_S);
  assert_int_equal(sucess->end_time, s->TEST_END_TIME_S);

  assert_non_null(sucess->duration);
  assert_int_equal(sucess->duration, s->TEST_END_TIME_S - s->TEST_START_TIME_S);

  free_timer_result(sucess);
  reset(&t);
}

void test_timer_stop_NULL(void **state) {
  // Given
  // When
  TimerResult *fail_null_timer = stop(NULL);

  // Then
  assert_null(fail_null_timer);
}

void test_timer_not_started_stop(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  TimerResult *fail_not_started = stop(&t);

  // Then
  assert_null(fail_not_started);

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
  assert_non_null(name);
  assert_string_equal(s->default_test_info->name, name);

  reset(&t);
  free(name);
}

void test_timer_NULL_get_name(void **state) {
  // Given
  // When
  char *null = get_name(NULL);

  // Then
  assert_null(null);
}

void test_timer_not_started_get_name(void **state) {
  // Given
  Timer t;
  // When
  char *not_started = get_name(&t);

  // Then
  assert_null(not_started);
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
  reset(&t);

  // Then
  assert_non_null(client);
  assert_string_equal(s->default_test_info->client, client);

  reset(&t);
  free(client);
}

void test_timer_NULL_get_client(void **state) {
  // Given
  // When
  char *null = get_client(NULL);

  // Then
  assert_null(null);
}

void test_timer_not_started_get_client(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = get_client(&t);

  // Then
  assert_null(not_started);

  // Finally
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
  assert_non_null(project);
  assert_string_equal(s->default_test_info->project, project);

  reset(&t);
  free(project);
}

void test_timer_NULL_get_project(void **state) {
  // Given
  // When
  char *null = get_project(NULL);

  // Then
  assert_null(null);

  // Finally
}

void test_timer_not_started_get_project(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = get_project(&t);

  // Then
  assert_null(not_started);

  // Finally
  reset(&t);
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
  assert_non_null(description);
  assert_string_equal(s->default_test_info->description, description);

  reset(&t);
  free(description);
}

void test_timer_NULL_get_description(void **state) {
  // Given
  // When
  char *null = get_description(NULL);

  // Then
  assert_null(null);

  // Finally
}

void test_timer_not_started_get_description(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = get_description(&t);

  // Then
  assert_null(not_started);

  // Finally
  reset(&t);
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
  assert_int_equal(duration,
                   s->TEST_END_TIME_S -
                       s->TEST_START_TIME_S); // duration = 38m 19s

  // Finally
  reset(&t);
}

void test_timer_NULL_get_duration(void **state) {
  // Given
  // When
  int null = get_duration(NULL);

  // Then
  assert_int_equal(0, null);

  // Finally
}

void test_timer_not_started_get_duration(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  int not_started = get_duration(&t);

  // Then
  assert_int_equal(0, not_started);

  // Finally
  reset(&t);
}
