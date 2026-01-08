#ifndef UNIT_TEST
#define UNIT_TEST

#include <stdarg.h>
#include <stddef.h>

// Order is important here, cmocka >> timer.h
#include <cmocka.h>
#ifdef stop
#undef stop
#endif // stop

#include "../src/timer.h"

time_t __wrap_time(time_t *__timer);
int check_pointer(CMockaValueData value, CMockaValueData expected);

typedef struct {
  StartInfo *default_start_info;
  UpdateInfo *default_update_info;
  char *TEST_ACTIVITY_1;
  char *TEST_ACTIVITY_2;
  char *TEST_CLIENT;
  char *TEST_PROJECT;
  time_t TEST_DURATION_S;
  time_t TEST_START_TIME_S;
  time_t TEST_END_TIME_S;
  struct mg_str *TEST_START_INFO_HTTP_REQUEST_BODY;
  struct mg_str *TEST_UPDATE_INFO_HTTP_REQUEST_BODY;
  struct mg_str *NOT_SET_TEST_HTTP_REQUEST_BODY;
} test_state_t;

#endif // !UNIT_TEST
