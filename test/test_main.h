#ifndef TEST_MAIN
#define TEST_MAIN

#include <bson/bson.h>
#include <setjmp.h>
#include <stdarg.h>

#include <stddef.h>

#include <cmocka.h>

#include "../src/timer.h"

typedef struct {
  time_t TEST_START_TIME_MS;
  time_t TEST_END_TIME_MS; //
  StartInfo *default_test_info;
  char *KEY;
  char *VALUE;
  bson_t *test_document;
} test_state_t;

#endif // !TEST_MAIN
