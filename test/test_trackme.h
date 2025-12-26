#ifndef TRACKME_H
#define TRACKME_H

#include "unit_test.h"

// Maybe move these to common shared state?
#define NO_END_TIME "No end time - stop timer first"
#define EXPECTED_DURATION "01:23:12"
#define EXPECTED_START_TIME "Mon, 22 Jul 2024 18:09:54 +0200"
#define EXPECTED_END_TIME "Mon, 22 Jul 2024 19:33:06 +0200"

// Internal symbols from trackme.c
extern StartInfo *_start_info_from_request_body(struct mg_str *);
extern TimerResult *current_timer_result;
extern char *_duration_int_to_string(int);
extern char *_time_t_to_str(time_t *);
extern int _get_duration_int();


/* Private functions */
void test_trackme_from_request_body(void **);
void test_trackme_from_NULL_request_body(void **);
void test_trackme_from_empty_request_body(void **);
void test_trackme_time_t_to_string(void **);
void test_trackme_time_t_null_to_string(void **);
void test_trackme_get_duration_int_started(void **);
void test_trackme_get_duration_int_not_started(void **);
void test_trackme_get_duration_int_stopped(void **);
void test_trackme_duration_int_to_string(void **);
void test_trackme_duration_negative_int_to_string(void **);

/* Public functions */
void test_trackme_start_timer_started(void **);
void test_trackme_start_timer_not_started(void **);
void test_trackme_is_timer_running_started(void **);
void test_trackme_is_timer_running_not_started(void **);
void test_trackme_is_timer_running_stopped(void **);
void test_trackme_stop_timer_started(void **);
void test_trackme_stop_timer_not_started(void **);
void test_trackme_get_start_time_started(void **);
void test_trackme_get_start_time_not_started(void **);
void test_trackme_get_start_time_stopped(void **);
void test_trackme_get_end_time_started(void **);
void test_trackme_get_end_time_not_started(void **);
void test_trackme_get_end_time_stopped(void **);
void test_trackme_get_duration_not_started(void **);
void test_trackme_get_duration_started(void **);
void test_trackme_get_duration_stopped(void **);
void test_trackme_get_activity_not_started(void **);
void test_trackme_get_activity_started_not_set(void **);
void test_trackme_get_activity_started_set(void **);
void test_trackme_get_activity_stopped(void **);
void test_trackme_get_client_not_started(void **);
void test_trackme_get_client_started_not_set(void **);
void test_trackme_get_client_started_set(void **);
void test_trackme_get_client_stopped(void **);
void test_trackme_get_project_not_started(void **);
void test_trackme_get_project_started_not_set(void **);
void test_trackme_get_project_started_set(void **);
void test_trackme_get_project_stopped(void **);
void test_trackme_get_description_not_started(void **);
void test_trackme_get_description_started_not_set(void **);
void test_trackme_get_description_started_set(void **);
void test_trackme_get_description_stopped(void **);

#endif // !TRACKME_H
