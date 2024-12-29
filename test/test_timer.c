#include "unit_test.h"

#include "../src/timer.h"
#include "../src/track_me.h"
#include <cmocka.h>
#include <stdlib.h>

// Helper functions
// Make sure to pass newly allocated memory so that we can safely free() it later.
StartInfo *_copy_start_info(StartInfo *orig_si) {
  if (!orig_si) {
    fail();
  }
  // init StartInfo struct
  StartInfo *si = calloc(1, sizeof(StartInfo));
  if (!si) {
    goto fail_test;
  }

  char *name = malloc(REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!name) {
    free(si);
    goto fail_test;
  }
  char *client = malloc(REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!client) {
    free(si);
    free(name);
    goto fail_test;
  }
  char *project = malloc(REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!project) {
    free(si);
    free(name);
    free(client);
    goto fail_test;
  }
  char *description = malloc(10 * REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!description) {
    free(si);
    free(name);
    free(client);
    free(project);
    goto fail_test;
  }
  si->name = name;
  si->client = client;
  si->project = project;
  si->description = description;

  // Fill StartInfo struct
  snprintf(name, strlen(orig_si->name) + 1, "%s", orig_si->name);
  snprintf(client, strlen(orig_si->client) + 1, "%s", orig_si->client);
  snprintf(project, strlen(orig_si->project) + 1, "%s", orig_si->project);
  snprintf(description, strlen(orig_si->description) + 1, "%s", orig_si->description);

  return si;
fail_test:
  fail();
};

/*** Tests start ***/
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
  assert_int_equal(0, t.start_time);
}

void test_timer_start(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  bool success = start(&t, _copy_start_info(s->default_test_info));

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
  StartInfo *si = _copy_start_info(s->default_test_info);

  // When
  bool fail_null_timer = start(NULL, si);

  // Then
  assert_false(fail_null_timer);

  // Finally
  free_start_info(si);
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
}

void test_timer_stop_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_test_info));
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S); // diff = 38m 19s

  // When
  TimerResult *stopped_result = stop(&t);

  // Then
  assert_non_null(stopped_result);

  assert_ptr_not_equal(s->default_test_info->name, stopped_result->name);
  assert_ptr_not_equal(s->default_test_info->client, stopped_result->client);
  assert_ptr_not_equal(s->default_test_info->project, stopped_result->project);
  assert_ptr_not_equal(s->default_test_info->description, stopped_result->description);

  assert_string_equal(s->default_test_info->name, stopped_result->name);
  assert_string_equal(s->default_test_info->client, stopped_result->client);
  assert_string_equal(s->default_test_info->project, stopped_result->project);
  assert_string_equal(s->default_test_info->description, stopped_result->description);

  assert_int_equal(stopped_result->start_time, s->TEST_START_TIME_S);
  assert_int_equal(stopped_result->end_time, s->TEST_END_TIME_S);

  assert_non_null(stopped_result->duration);
  assert_int_equal(stopped_result->duration, s->TEST_END_TIME_S - s->TEST_START_TIME_S);

  // Finally
  free_timer_result(stopped_result);
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

  // Finally
  reset(&t);
}

void test_timer_get_name(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_test_info));

  // When
  char *name = t.name;

  // Then
  assert_non_null(name);
  assert_string_equal(s->default_test_info->name, name);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_name(void **state) {
  // Given
  Timer t;
  reset(&t);
  // When
  char *not_started = t.name;

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
  start(&t, _copy_start_info(s->default_test_info));

  // When
  char *client = t.client;

  // Then
  assert_non_null(client);
  assert_string_equal(s->default_test_info->client, client);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_client(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = t.client;

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
  start(&t, _copy_start_info(s->default_test_info));

  // When
  char *project = t.project;

  // Then
  assert_non_null(project);
  assert_string_equal(s->default_test_info->project, project);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_project(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = t.project;

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
  start(&t, _copy_start_info(s->default_test_info));

  // When
  char *description = t.description;

  // Then
  assert_non_null(description);
  assert_string_equal(s->default_test_info->description, description);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_description(void **state) {
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = t.description;

  // Then
  assert_null(not_started);

  // Finally
  reset(&t);
}
