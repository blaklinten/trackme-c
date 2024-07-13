#include "../src/db.h"
#include "test_main.h"
#include <bson/bson.h>

void db_connect() {
  bson_t *command_ptr = NULL, reply;
  bson_error_t error;
  bool sucess = false;

  /*
   * Do work. This example pings the database.
   */
  command_ptr = BCON_NEW("ping", BCON_INT32(1));

  sucess = mongoc_client_command_simple(db_client_ptr, "admin", command_ptr,
                                        NULL, &reply, &error);

  if (!sucess) {
    fprintf(stderr, "Failed to ping db\n");
    fail();
  }

  /*
   * Release our handles and clean up libmongoc
   */
  bson_destroy(&reply);
  bson_destroy(command_ptr);
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
