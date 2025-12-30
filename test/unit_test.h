#ifndef UNIT_TEST
#define UNIT_TEST

#include <bson/bson.h>
#include <setjmp.h>
#include <stdarg.h>

#include <stddef.h>

#include <cmocka.h>

#include "../src/timer.h"

time_t __wrap_time(time_t *__timer);

typedef struct {
  StartInfo *default_start_info;
  UpdateInfo *default_update_info;
  bson_t *test_document_1;
  bson_t *test_document_2;
  bson_oid_t test_id_1;
  bson_oid_t test_id_2;
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
