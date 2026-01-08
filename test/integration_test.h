#ifndef INTEGRATION_TEST
#define INTEGRATION_TEST

#include <cmocka.h>
#ifdef stop
#undef stop
#endif // stop
#include "../src/timer.h"
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
#include <time.h>

typedef struct {
  char *TEST_ACTIVITY_1;
  char *TEST_ACTIVITY_2;
  char *TEST_CLIENT;
  char *TEST_PROJECT;
  time_t TEST_DURATION_S;
  time_t TEST_START_TIME_S;
  time_t TEST_END_TIME_S;
} test_state_t;

#endif // !INTEGRATION_TEST
