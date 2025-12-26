#include "unit_test.h"

#include "../src/timer.h"
#include "../src/track_me.h"
#include <cmocka.h>
#include <stdlib.h>

/*** Helper functions ***/
// Make sure to pass newly allocated memory so that we can safely free() it later.
UpdateInfo *_copy_update_info(UpdateInfo *orig_ui) {
  if (!orig_ui) {
    fail();
  }
  // init UpdateInfo struct
  UpdateInfo *ui = calloc(1, sizeof(UpdateInfo));
  if (!ui) {
    goto fail_test;
  }

  char *activity = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!activity) {
    free(ui);
    goto fail_test;
  }
  char *client = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!client) {
    free(ui);
    free(activity);
    goto fail_test;
  }
  char *project = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!project) {
    free(ui);
    free(activity);
    free(client);
    goto fail_test;
  }
  char *description = malloc(REQUEST_FIELD_LONG_SIZE * sizeof(char));
  if (!description) {
    free(ui);
    free(activity);
    free(client);
    free(project);
    goto fail_test;
  }
  ui->info.activity = activity;
  ui->info.client = client;
  ui->info.project = project;
  ui->info.description = description;

  // Fill StartInfo struct
  snprintf(activity, REQUEST_FIELD_SHORT_SIZE, "%s", orig_ui->info.activity);
  snprintf(client, REQUEST_FIELD_SHORT_SIZE, "%s", orig_ui->info.client);
  snprintf(project, REQUEST_FIELD_SHORT_SIZE, "%s", orig_ui->info.project);
  snprintf(description, REQUEST_FIELD_LONG_SIZE, "%s", orig_ui->info.description);
  ui->start_time = orig_ui->start_time;
  ui->end_time = orig_ui->end_time;

  return ui;
fail_test:
  fail();
};

StartInfo *_copy_start_info(StartInfo *orig_si) {
  if (!orig_si) {
    fail();
  }
  // init StartInfo struct
  StartInfo *si = calloc(1, sizeof(StartInfo));
  if (!si) {
    goto fail_test;
  }

  char *activity = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!activity) {
    free(si);
    goto fail_test;
  }
  char *client = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!client) {
    free(si);
    free(activity);
    goto fail_test;
  }
  char *project = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!project) {
    free(si);
    free(activity);
    free(client);
    goto fail_test;
  }
  char *description = malloc(REQUEST_FIELD_LONG_SIZE * sizeof(char));
  if (!description) {
    free(si);
    free(activity);
    free(client);
    free(project);
    goto fail_test;
  }
  si->activity = activity;
  si->client = client;
  si->project = project;
  si->description = description;

  // Fill StartInfo struct
  snprintf(activity, REQUEST_FIELD_SHORT_SIZE, "%s", orig_si->activity);
  snprintf(client, REQUEST_FIELD_SHORT_SIZE, "%s", orig_si->client);
  snprintf(project, REQUEST_FIELD_SHORT_SIZE, "%s", orig_si->project);
  snprintf(description, REQUEST_FIELD_LONG_SIZE, "%s", orig_si->description);

  return si;
fail_test:
  fail();
};

/*** Tests begin ***/
// Timer
void test_timer_reset(void **state) {
  (void) state;
  // Given
  Timer t;
  // When
  reset(&t);

  // Then
  assert_null(t.info.activity);
  assert_null(t.info.client);
  assert_null(t.info.project);
  assert_null(t.info.description);
  assert_int_equal(0, t.start_time);
}

void test_timer_free_timer_result(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_start_info));

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  TimerResult *stopped_result = stop(&t);

  // When
  free_timer_result(stopped_result);

  // Then
  /* No memory leaks indicate successful free */

  // Finally
}

void test_timer_free_start_info(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  StartInfo *si = _copy_start_info(s->default_start_info);

  // When
  free_start_info(si);

  // Then
  /* No memory leaks indicate successful free */

  // Finally
}

void test_timer_free_update_info(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  UpdateInfo *ui = _copy_update_info(s->default_update_info);

  // When
  free_update_info(ui);

  // Then
  /* No memory leaks indicate successful free */

  // Finally
}

void test_timer_start(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  bool success = start(&t, _copy_start_info(s->default_start_info));

  // Then
  assert_true(success);

  assert_ptr_not_equal(s->default_start_info->activity, t.info.activity);
  assert_string_equal(s->default_start_info->activity, t.info.activity);

  assert_ptr_not_equal(s->default_start_info->client, t.info.client);
  assert_string_equal(s->default_start_info->client, t.info.client);

  assert_ptr_not_equal(s->default_start_info->project, t.info.project);
  assert_string_equal(s->default_start_info->project, t.info.project);

  assert_ptr_not_equal(s->default_start_info->description, t.info.description);
  assert_string_equal(s->default_start_info->description, t.info.description);

  assert_true(t.start_time > 0);
  assert_true(t.start_time == s->TEST_START_TIME_S);

  // Finally
  reset(&t);
}

void test_timer_start_NULL(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  StartInfo *si = _copy_start_info(s->default_start_info);

  // When
  bool fail_null_timer = start(NULL, si);

  // Then
  assert_false(fail_null_timer);

  // Finally
  free_start_info(si);
}
void test_timer_start_NULL_start_info(void **state) {
  (void) state;
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
  start(&t, _copy_start_info(s->default_start_info));

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S); // diff = 38m 19s

  // When
  TimerResult *stopped_result = stop(&t);

  // Then
  assert_non_null(stopped_result);

  assert_ptr_not_equal(s->default_start_info->activity, stopped_result->info.activity);
  assert_ptr_not_equal(s->default_start_info->client, stopped_result->info.client);
  assert_ptr_not_equal(s->default_start_info->project, stopped_result->info.project);
  assert_ptr_not_equal(s->default_start_info->description, stopped_result->info.description);

  assert_string_equal(s->default_start_info->activity, stopped_result->info.activity);
  assert_string_equal(s->default_start_info->client, stopped_result->info.client);
  assert_string_equal(s->default_start_info->project, stopped_result->info.project);
  assert_string_equal(s->default_start_info->description, stopped_result->info.description);

  assert_int_equal(stopped_result->start_time, s->TEST_START_TIME_S);
  assert_int_equal(stopped_result->end_time, s->TEST_END_TIME_S);

  assert_non_null(stopped_result->duration);
  assert_int_equal(stopped_result->duration, s->TEST_END_TIME_S - s->TEST_START_TIME_S);

  // Finally
  free_timer_result(stopped_result);
}

void test_timer_stop_NULL(void **state) {
  (void) state;
  // Given
  // When
  TimerResult *fail_null_timer = stop(NULL);

  // Then
  assert_null(fail_null_timer);
}

void test_timer_stop_not_started(void **state) {
  (void) state;
  // Given
  Timer t;
  reset(&t);

  // When
  TimerResult *fail_not_started = stop(&t);

  // Then
  assert_null(fail_not_started);

  // Finally
}

void test_timer_update(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_start_info));

  // When
  UpdateInfo *new_activity_and_start_time = _copy_update_info(s->default_update_info);

  /* edit only activity and start time, NULL/0 means no change */
  free(new_activity_and_start_time->info.client);
  new_activity_and_start_time->info.client = NULL;
  free(new_activity_and_start_time->info.project);
  new_activity_and_start_time->info.project = NULL;
  free(new_activity_and_start_time->info.description);
  new_activity_and_start_time->info.description = NULL;
  new_activity_and_start_time->end_time = 0;

  bool updated = update(&t, new_activity_and_start_time);

  // Then
  assert_true(updated);

  assert_ptr_not_equal(new_activity_and_start_time->info.activity, t.info.activity);
  assert_string_equal(new_activity_and_start_time->info.activity, t.info.activity);
  assert_ptr_not_equal(s->default_start_info->activity, t.info.activity);
  assert_string_not_equal(s->default_start_info->activity, t.info.activity);

  assert_true(t.start_time > 0);
  assert_int_equal(t.start_time, new_activity_and_start_time->start_time);
  assert_int_not_equal(t.start_time, s->TEST_START_TIME_S);

  assert_ptr_not_equal(s->default_start_info->client, t.info.client);
  assert_string_equal(s->default_start_info->client, t.info.client);

  assert_ptr_not_equal(s->default_start_info->project, t.info.project);
  assert_string_equal(s->default_start_info->project, t.info.project);

  assert_ptr_not_equal(s->default_start_info->description, t.info.description);
  assert_string_equal(s->default_start_info->description, t.info.description);

  // Finally
  free_update_info(new_activity_and_start_time);
  reset(&t);
}

void test_timer_update_NULL(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  UpdateInfo *ui = _copy_update_info(s->default_update_info);

  // When
  bool fail_null_timer = update(NULL, ui);

  // Then
  assert_false(fail_null_timer);

  // Finally
  free_update_info(ui);
}

void test_timer_update_NULL_update_info(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_start_info));

  // When
  bool fail_null_update_info = update(&t, NULL);

  // Then
  assert_false(fail_null_update_info);

  // Finally
  reset(&t);
}

void test_timer_update_not_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  UpdateInfo *ui = _copy_update_info(s->default_update_info);

  // When
  bool fail_not_started = update(&t, ui);

  // Then
  assert_false(fail_not_started);

  // Finally
  free_update_info(ui);
  reset(&t);
}

void test_timer_get_activity(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_start_info));

  // When
  char *activity = t.info.activity;

  // Then
  assert_non_null(activity);
  assert_string_equal(s->default_start_info->activity, activity);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_activity(void **state) {
  (void) state;
  // Given
  Timer t;
  reset(&t);
  // When
  char *not_started = t.info.activity;

  // Then
  assert_null(not_started);

  // Finally
}

void test_timer_get_client(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_start_info));

  // When
  char *client = t.info.client;

  // Then
  assert_non_null(client);
  assert_string_equal(s->default_start_info->client, client);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_client(void **state) {
  (void) state;
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = t.info.client;

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
  start(&t, _copy_start_info(s->default_start_info));

  // When
  char *project = t.info.project;

  // Then
  assert_non_null(project);
  assert_string_equal(s->default_start_info->project, project);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_project(void **state) {
  (void) state;
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = t.info.project;

  // Then
  assert_null(not_started);

  // Finally
}

void test_timer_get_description(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  Timer t;
  reset(&t);
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start(&t, _copy_start_info(s->default_start_info));

  // When
  char *description = t.info.description;

  // Then
  assert_non_null(description);
  assert_string_equal(s->default_start_info->description, description);

  // Finally
  reset(&t);
}

void test_timer_not_started_get_description(void **state) {
  (void) state;
  // Given
  Timer t;
  reset(&t);

  // When
  char *not_started = t.info.description;

  // Then
  assert_null(not_started);

  // Finally
}
