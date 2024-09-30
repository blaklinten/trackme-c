#ifndef TRACKME_H
#define TRACKME_H

void test_trackme_get_current_duration_str(void **);
void test_trackme_get_current_duration_str_not_started(void **);
void test_trackme_get_current_duration_str_NULL(void **);
void test_trackme_get_final_duration_str_NULL(void **);
void test_trackme_get_final_duration_str_not_started(void **);
void test_trackme_get_final_duration_str(void **);
void test_trackme_get_start_time_str(void **);
void test_trackme_get_stop_time_str(void **);
void test_trackme_from_request_body(void **);
void test_trackme_from_NULL_request_body(void **);
void test_trackme_from_empty_request_body(void **);

#endif // !TRACKME_H
