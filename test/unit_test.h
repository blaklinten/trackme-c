#ifndef UNIT_TEST
#define UNIT_TEST

#include <setjmp.h>
#include <stdarg.h>

#include <stddef.h>

#include <cmocka.h>

#include "../src/timer.h"

typedef struct {
  time_t TEST_START_TIME_MS;
  time_t TEST_END_TIME_MS; //
  StartInfo *default_test_info;
  char *TEST_NAME_1;
  char *TEST_NAME_2;
} test_state_t;

#endif // !UNIT_TEST
