#ifndef TEST_TIMER_H
#define TEST_TIMER_H

void test_timer_reset(void **);
void test_timer_free_timer_result(void **);
void test_timer_free_start_info(void **);
void test_timer_start(void **);
void test_timer_start_NULL(void **);
void test_timer_start_NULL_start_info(void **);
void test_timer_stop_started(void **);
void test_timer_stop_NULL(void **);
void test_timer_stop_not_started(void **);
void test_timer_get_name(void **);
void test_timer_not_started_get_name(void **);
void test_timer_get_client(void **);
void test_timer_not_started_get_client(void **);
void test_timer_get_project(void **);
void test_timer_not_started_get_project(void **);
void test_timer_get_description(void **);
void test_timer_not_started_get_description(void **);
void test_timer_get_duration(void **);
void test_timer_not_started_get_duration(void **);

#endif // !TEST_TIMER_H
