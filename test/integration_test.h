#ifndef INTEGRATION_TEST
#define INTEGRATION_TEST

#include <cmocka.h>
#ifdef stop
#undef stop
#endif // stop
#include "../lib/sqlite3.h"
#include "../src/timer.h"
#include <stdarg.h>
#include <stddef.h>

typedef struct {
  TimerResult *TEST_TIMER_RESULT;
  StartInfo *default_start_info;
  sqlite3 *test_handle;
} test_state_t;

#endif // !INTEGRATION_TEST
