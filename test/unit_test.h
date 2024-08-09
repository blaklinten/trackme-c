#ifndef UNIT_TEST
#define UNIT_TEST

#include <bson/bson.h>
#include <setjmp.h>
#include <stdarg.h>

#include <stddef.h>

#include <cmocka.h>

#include "../src/timer.h"

typedef struct {
  StartInfo *default_test_info;
  bson_t *test_document_1;
  bson_t *test_document_2;
  bson_oid_t test_id_1;
  bson_oid_t test_id_2;
  char *TEST_NAME_1;
  char *TEST_NAME_2;
  char *TEST_CLIENT;
  char *TEST_PROJECT;
  time_t TEST_DURATION_S;
  time_t TEST_START_TIME_S;
  time_t TEST_END_TIME_S;
} test_state_t;

#endif // !UNIT_TEST
