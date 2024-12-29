#ifndef TEST_TIMER_H
#define TEST_TIMER_H

void test_timer_reset();
void test_timer_start();
void test_timer_start_NULL();
void test_timer_start_NULL_start_info();
void test_timer_stop_started();
void test_timer_stop_NULL();
void test_timer_not_started_stop();
void test_timer_get_name();
void test_timer_NULL_get_name();
void test_timer_not_started_get_name();
void test_timer_get_client();
void test_timer_NULL_get_client();
void test_timer_not_started_get_client();
void test_timer_get_project();
void test_timer_NULL_get_project();
void test_timer_not_started_get_project();
void test_timer_get_description();
void test_timer_NULL_get_description();
void test_timer_not_started_get_description();
void test_timer_get_duration();
void test_timer_NULL_get_duration();
void test_timer_not_started_get_duration();

#endif // !TEST_TIMER_H
