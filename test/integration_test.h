#ifndef INTEGRATION_TEST
#define INTEGRATION_TEST

#include <bson/bson.h>
#include <setjmp.h>
#include <stdarg.h>

#include <stddef.h>

#include <cmocka.h>

typedef struct {
  bson_t *test_document;
  bson_oid_t test_id;
  char *TEST_NAME_1;
  char *TEST_NAME_2;
} test_state_t;

#endif // !INTEGRATION_TEST
