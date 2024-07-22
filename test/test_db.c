#include "test_db.h"
#include "../src/db.h"
#include "../src/log.h"
#include "../src/track_me.h"
#include "integration_test.h"
#include <bson/bson.h>
#include <cmocka.h>
#include <mongoc/mongoc.h>

void test_db_connect() {
  // Given
  bson_t *command = NULL, reply;
  bson_error_t error;
  bool sucess = false;
  command = BCON_NEW("ping", BCON_INT32(1));

  // When
  sucess = mongoc_client_command_simple(db_client, "admin", command, NULL,
                                        &reply, &error);
  // Then
  if (!sucess) {
    t_log(ERROR, __func__, "Failed to ping db");
    fail();
  }

  // Finally
  bson_destroy(&reply);
  bson_destroy(command);
}

// Mock mongoc_collection_insert_one(mongoc_collection_t *collection, const
// bson_t *document, const bson_t *opts, bson_t *reply, bson_error_t *error)
bool __wrap_mongoc_collection_insert_one(mongoc_collection_t *collection_ptr,
                                         const bson_t *document_ptr,
                                         const bson_t *opts_ptr,
                                         bson_t *reply_ptr,
                                         bson_error_t *error_ptr) {
  check_expected_ptr(collection_ptr);
  function_called();
  return mock();
}

void db_save(void **state) {
  // Given
  bson_t *test_document_ptr = ((test_state_t *)*state)->test_document;
  expect_value(__wrap_mongoc_collection_insert_one, collection_ptr,
               entries_ptr);
  will_return(__wrap_mongoc_collection_insert_one, true);
  expect_function_call(__wrap_mongoc_collection_insert_one);

  // When
  bool sucess = save(test_document_ptr);

  // Then
  assert_true(sucess);
}
