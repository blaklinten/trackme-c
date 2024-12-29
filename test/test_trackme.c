#include "test_trackme.h"
#include "../lib/mongoose.h"
#include "../src/track_me.h"
#include "../src/util/log.h"
#include <bson/bson.h>
#include <cmocka.h>

/*** Internal helper test functions ***/

char **_get_start_info_names(StartInfo *si1, StartInfo *si2) {
  char **ret = malloc(2 * sizeof(char *));
  ret[0] = si1->name;
  ret[1] = si2->name;
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
  char **names = _get_start_info_names(si1, si2);
  t_log(INFO, __func__, "Names are [%s] and [%s]", names[0], names[1]);
  assert_string_equal(names[0], names[1]);
  free(names);

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

/*** Tests begin ***/
/*** Internal functions ***/
void test_trackme_from_request_body(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  StartInfo *si = _start_info_from_request_body(s->TEST_HTTP_REQUEST_BODY);

  // Then
  assert_non_null(si);
  assert_true(_compare_start_info(s->default_test_info, si));

  // Finally
  free_start_info(si);
}

void test_trackme_from_NULL_request_body(void **state) {
  // Given

  // When
  StartInfo *fail_null = _start_info_from_request_body(NULL);

  // Then
  assert_null(fail_null);

  // Finally
}

void test_trackme_from_empty_request_body(void **state) {
  // Given
  struct mg_str invalid_body = mg_str("");

  // When
  StartInfo *fail_empty = _start_info_from_request_body(&invalid_body);

  // Then
  assert_non_null(fail_empty);

  // Finally
  free_start_info(fail_empty);
}

void test_trackme_time_t_to_string(void ** state) {
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

void test_trackme_time_t_null_to_string(void **state){
  // Given

  // When
  char *start_time_char = _time_t_to_str(NULL);

  // Then
  assert_non_null(start_time_char);
  assert_string_equal(start_time_char, NULL_TIME);

  // Finally
  free(start_time_char);
}

void test_trackme_get_duration_int_started(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  int duration_sec = _get_duration_int();

  // Then
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_int_equal(duration_sec, s->TEST_DURATION_S);

  // Finally
}

void test_trackme_get_duration_int_not_started(void **state){
  // Given

  // When
  int duration_sec = _get_duration_int();

  // Then
  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_int_equal(duration_sec, 0);

  // Finally
}

void test_trackme_get_duration_int_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
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
void test_trackme_duration_int_to_string(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  // When
  char* duration_str = _duration_int_to_string(s->TEST_DURATION_S);


  // Then
  assert_non_null(duration_str);
  assert_string_equal(duration_str, EXPECTED_DURATION);

  // Finally
  free(duration_str);
}

void test_trackme_duration_negative_int_to_string(void **state){
  // Given

  // When
  char* duration_str = _duration_int_to_string(-1);

  // Then
  assert_null(duration_str);

  // Finally
  free(duration_str);
}

/* Public functions */

void test_trackme_start_timer_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  // return end time first time - not the start time we want
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // return start time second time - the start time we do want!
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  char *name = get_name();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(name, s->default_test_info->name);
  assert_string_equal(client, s->default_test_info->client);
  assert_string_equal(project, s->default_test_info->project);
  assert_string_equal(description, s->default_test_info->description);
  assert_string_equal(start, EXPECTED_START_TIME);
  assert_string_equal(end, NO_END_TIME);
  assert_string_equal(duration, EXPECTED_DURATION);// If timer was not restarted with correct start time this should fail as duration would be 0

  // Finally
  free(name);
  free(start);
  free(description);
  free(project);
  free(client);
  free(end);
  free(duration);
}

void test_trackme_start_timer_not_started(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);

  // When
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  char *name = get_name();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  char *duration = get_duration();

  // Then
  assert_null(current_timer_result);
  assert_true(is_timer_running());
  assert_string_equal(name, s->default_test_info->name);
  assert_string_equal(client, s->default_test_info->client);
  assert_string_equal(project, s->default_test_info->project);
  assert_string_equal(description, s->default_test_info->description);
  assert_string_equal(start, EXPECTED_START_TIME);
  assert_string_equal(end, NO_END_TIME);
  assert_string_equal(duration, EXPECTED_DURATION);

  // Finally
  free(name);
  free(client);
  free(project);
  free(description);
  free(start);
  free(end);
  free(duration);
}

void test_trackme_is_timer_running_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

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

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
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

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  char *name = get_name();
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
  assert_string_equal(name, s->default_test_info->name);
  assert_string_equal(client, s->default_test_info->client);
  assert_string_equal(project, s->default_test_info->project);
  assert_string_equal(description, s->default_test_info->description);
  assert_string_equal(start, expected_start);
  assert_string_equal(end, expected_end);
  assert_string_equal(duration, expected_duration);


  //Finally
  free(start);
  free(end);
  free(duration);
  free(expected_duration);
  free(expected_start);
  free(expected_end);
}

void test_trackme_stop_timer_not_started(void **state){
  // Given

  // When
  bool success = stop_timer();

  char *name = get_name();
  char *client = get_client();
  char *project = get_project();
  char *description = get_description();
  char *start = get_start_time();
  char *end = get_end_time();
  char *duration = get_duration();

  // Then
  assert_false(success); // timer is stopped but no result is created; something is wrong (i.e. not started)!
  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_string_equal(name, NOT_SET);
  assert_string_equal(client, NOT_SET);
  assert_string_equal(project, NOT_SET);
  assert_string_equal(description, NOT_SET);
  assert_string_equal(start, NULL_TIME);
  assert_string_equal(end, NO_END_TIME);
  assert_string_equal(duration, ZERO_STR);

  //Finally
  free(name);
  free(client);
  free(project);
  free(description);
  free(start);
  free(end);
  free(duration);
}

void test_trackme_get_start_time_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  char *start_time = get_start_time();
  char *end_time = get_end_time();

  expect_value(__wrap_time, __timer, NULL);
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
  assert_string_equal(start_time, NULL_TIME);

  assert_null(current_timer_result);
  assert_false(is_timer_running());
  assert_string_equal(end_time, NO_END_TIME);
  assert_string_equal(duration, ZERO_STR);

  // Finally
  free(start_time);
  free(end_time);
  free(duration);
}

void test_trackme_get_start_time_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
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

void test_trackme_get_end_time_started(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  char *end_time = get_end_time();

  expect_value(__wrap_time, __timer, NULL);
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

void test_trackme_get_end_time_not_started(void **state){
  // Given

  // When
  char *end_time = get_end_time();

  // Then
  assert_string_equal(end_time, NO_END_TIME);
  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(end_time);
}

void test_trackme_get_end_time_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
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
  assert_string_equal(duration, ZERO_STR);
  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(duration);
}

void test_trackme_get_duration_started(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  expect_value(__wrap_time, __timer, NULL);
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

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
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

void test_trackme_get_name_not_started(void **state){
  // Given
  // When
  char *name = get_name();

  // Then
  assert_string_equal(name, NOT_SET);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(name);
}

void test_trackme_get_name_started_not_set(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->NOT_SET_TEST_HTTP_REQUEST_BODY);

  // When
  char *name = get_name();

  // Then
  assert_string_equal(name, NOT_SET);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(name);
}

void test_trackme_get_name_started_set(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  char *name = get_name();

  // Then
  assert_string_equal(name, s->default_test_info->name);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(name);
}

void test_trackme_get_name_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *name = get_name();

  // Then
  assert_string_equal(name, s->default_test_info->name);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  // TODO this should be freed when implementation for get_xxx() is fixed
  // free(name);

}

void test_trackme_get_client_not_started(void **state){
  // Given
  // When
  char *client = get_client();

  // Then
  assert_string_equal(client, NOT_SET);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(client);
}

void test_trackme_get_client_started_not_set(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
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

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  char *client = get_client();

  // Then
  assert_string_equal(client, s->default_test_info->client);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(client);
}

void test_trackme_get_client_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *client = get_client();

  // Then
  assert_string_equal(client, s->default_test_info->client);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  // TODO this should be freed when implementation for get_xxx() is fixed
  // free(client);
}


void test_trackme_get_project_not_started(void **state){
  // Given
  // When
  char *project = get_project();

  // Then
  assert_string_equal(project, NOT_SET);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(project);
}

void test_trackme_get_project_started_not_set(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
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

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  char *project = get_project();

  // Then
  assert_string_equal(project, s->default_test_info->project);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(project);
}

void test_trackme_get_project_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *project = get_project();

  // Then
  assert_string_equal(project, s->default_test_info->project);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  // TODO this should be freed when implementation for get_xxx() is fixed
  // free(project);
}


void test_trackme_get_description_not_started(void **state){
  // Given
  // When
  char *description = get_description();

  // Then
  assert_string_equal(description, NOT_SET);

  assert_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  free(description);
}

void test_trackme_get_description_started_not_set(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
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

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  // When
  char *description = get_description();

  // Then
  assert_string_equal(description, s->default_test_info->description);
  assert_null(current_timer_result);
  assert_true(is_timer_running());

  // Finally
  free(description);
}

void test_trackme_get_description_stopped(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_START_TIME_S);
  start_timer(s->TEST_HTTP_REQUEST_BODY);

  expect_value(__wrap_time, __timer, NULL);
  will_return(__wrap_time, s->TEST_END_TIME_S);
  stop_timer();

  // When
  char *description = get_description();

  // Then
  assert_string_equal(description, s->default_test_info->description);
  assert_non_null(current_timer_result);
  assert_false(is_timer_running());

  // Finally
  // TODO this should be freed when implementation for get_xxx() is fixed
  // free(description);
}
