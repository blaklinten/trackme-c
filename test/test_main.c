#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

#include "../src/timer.h"

const time_t TEST_START_TIME_MS = 1690876956; // Tue  1 Aug 10:02:36 CEST 2023
const time_t TEST_END_TIME_MS = 1690879255;   // Tue  1 Aug 10:40:55 CEST 2023

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
static void timer_create_timer_test() {
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

static void timer_start_test() {
  // Given
  StartInfo si = {"test_name", "test_client", "test_project",
                  "this is a test description"};
  Timer t;
  reset(&t);

  // When
  expect_value(__wrap_time, __timer, &t.start_time);
  will_return(__wrap_time, TEST_START_TIME_MS);

  ErrorCode started = start(&t, &si);

  // Then
  assert_int_equal(started, E_OK);
  assert_string_equal(si.name, t.name);
  assert_string_equal(si.client, t.client);
  assert_string_equal(si.project, t.project);
  assert_string_equal(si.description, t.description);
  assert_true(t.start_time > 0);
  assert_true(t.start_time == TEST_START_TIME_MS);
}

static void timer_stop_test() {
  // Given
  StartInfo si = {"test_name", "test_client", "test_project",
                  "this is a test description"};
  Timer t;
  reset(&t);

  expect_value(__wrap_time, __timer, &t.start_time);
  will_return(__wrap_time, TEST_START_TIME_MS);
  (void)start(&t, &si);

  // When
  TimerResult tr;
  expect_value(__wrap_time, __timer, &tr.end_time);
  will_return(__wrap_time, TEST_END_TIME_MS); // diff = 38m 19s

  ErrorCode stopped = stop(&t, &tr);

  // Then
  assert_int_equal(stopped, E_OK);
  assert_string_equal(tr.name, si.name);
  assert_string_equal(tr.client, si.client);
  assert_string_equal(tr.project, si.project);
  assert_string_equal(tr.description, si.description);
  assert_true(t.start_time == tr.start_time);
  assert_int_equal(tr.start_time, TEST_START_TIME_MS);
  assert_int_equal(tr.end_time, TEST_END_TIME_MS);
  assert_non_null(tr.duration);
  assert_int_equal(tr.duration, TEST_END_TIME_MS - TEST_START_TIME_MS);
}

int main(void) {

  const struct CMUnitTest tests[] = {
      cmocka_unit_test(timer_create_timer_test),
      cmocka_unit_test(timer_start_test),
      cmocka_unit_test(timer_stop_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
