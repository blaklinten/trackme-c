#include "integration_test.h"
#include "../src/db.h"
#include "../src/track_me.h"
#include "test_db.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>

static int group_setup(void **state) {
  test_state_t *s = malloc(sizeof(test_state_t));


  const char *uri_string = "mongodb://blaklinten:test-psw@localhost:27017";
  init_db(uri_string);
  mongoc_collection_drop(entries, NULL);

  s->test_document = bson_new();
  bson_oid_init(&s->test_id, NULL);
  BSON_APPEND_OID(s->test_document, DB_KEY_ID, &s->test_id);
  s->TEST_NAME_1 = "Lucas";
  s->TEST_NAME_2 = "Jonas";
  BSON_APPEND_UTF8(s->test_document, DB_KEY_NAME, s->TEST_NAME_1);

  *state = s;

  return 0;
}

static int group_teardown(void **state) {
  test_state_t *s = (test_state_t *)*state;

  if (s) {
    if (s->test_document) {
      bson_destroy(s->test_document);
    }
    free(s);
  }

  free_db();

  return 0;
}

int main(void) {

  const struct CMUnitTest integration_test[] = {
      cmocka_unit_test(test_db_connect),
      cmocka_unit_test(test_db_insert_and_get),
      cmocka_unit_test(test_db_save),
      cmocka_unit_test(test_db_get_by),
  };

  return cmocka_run_group_tests(integration_test, group_setup, group_teardown);
}
