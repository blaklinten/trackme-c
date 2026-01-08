#ifndef INTEGRATION_TEST
#define INTEGRATION_TEST

#include <cmocka.h>
#ifdef stop
#undef stop
#endif // stop
#include "../src/timer.h"
#include <stdarg.h>
#include <stddef.h>

typedef struct {
  TimerResult *TEST_TIMER_RESULT;
  StartInfo *default_start_info;
} test_state_t;

#endif // !INTEGRATION_TEST
