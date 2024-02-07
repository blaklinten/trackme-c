#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

#include "../src/timer.h"

const time_t TEST_START_TIME_MS = 1690876956; // Tue  1 Aug 10:02:36 CEST 2023
const time_t TEST_END_TIME_MS = 1690879255;   // Tue  1 Aug 10:40:55 CEST 2023

  StartInfo start_info = {"test_name", "test_client", "test_project",
                  "this is a test description"};

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
void timer_reset() {
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

void timer_start() {
  // Given
  Timer t;
  reset(&t);

  // When
  expect_value(__wrap_time, __timer, &t.start_time);
  will_return(__wrap_time, TEST_START_TIME_MS);

  ErrorCode started = start(&t, &start_info);

  // Then
  assert_int_equal(started, E_OK);
  assert_string_equal(start_info.name, t.name);
  assert_string_equal(start_info.client, t.client);
  assert_string_equal(start_info.project, t.project);
  assert_string_equal(start_info.description, t.description);
  assert_true(t.start_time > 0);
  assert_true(t.start_time == TEST_START_TIME_MS);
}

void timer_stop() {
  // Given
  Timer t;
  reset(&t);

  expect_value(__wrap_time, __timer, &t.start_time);
  will_return(__wrap_time, TEST_START_TIME_MS);
  (void)start(&t, &start_info);

  // When
  TimerResult tr;
  expect_value(__wrap_time, __timer, &tr.end_time);
  will_return(__wrap_time, TEST_END_TIME_MS); // diff = 38m 19s

  ErrorCode stopped = stop(&t, &tr);

  // Then
  assert_int_equal(stopped, E_OK);
  assert_string_equal(tr.name, start_info.name);
  assert_string_equal(tr.client, start_info.client);
  assert_string_equal(tr.project, start_info.project);
  assert_string_equal(tr.description, start_info.description);
  assert_true(t.start_time == tr.start_time);
  assert_int_equal(tr.start_time, TEST_START_TIME_MS);
  assert_int_equal(tr.end_time, TEST_END_TIME_MS);
  assert_non_null(tr.duration);
  assert_int_equal(tr.duration, TEST_END_TIME_MS - TEST_START_TIME_MS);
}

void timer_get_duration() {
  // Given
  Timer t;
  reset(&t);

  // When
  expect_value(__wrap_time, __timer, &t.start_time);
  will_return(__wrap_time, TEST_START_TIME_MS);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, TEST_END_TIME_MS);

  (void)start(&t, &start_info);

  time_t duration;
  ErrorCode err = get_duration(&t, &duration);

  // Then
  assert_int_equal(err, E_OK);
  assert_non_null(duration);
  assert_int_equal(duration, TEST_END_TIME_MS - TEST_START_TIME_MS);

}
