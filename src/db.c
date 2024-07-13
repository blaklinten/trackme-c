#include "db.h"
#include "log.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>

mongoc_collection_t *entries_ptr = NULL;
mongoc_client_t *db_client_ptr = NULL;

int init_db(const char *uri) {

  mongoc_init();
  bson_error_t error;
  mongoc_uri_t *m_uri_ptr = mongoc_uri_new_with_error(uri, &error);
  if (!m_uri_ptr) {
    t_log(ERROR, __func__,
          "failed to parse URI: %s\n"
          "error message:       %s\n",
          uri, error.message);
    return EXIT_FAILURE;
  }
  db_client_ptr = mongoc_client_new_from_uri(m_uri_ptr);
  if (!db_client_ptr) {
    fprintf(stderr, "failed to get db_client \n");
    return EXIT_FAILURE;
  }
  mongoc_client_set_appname(db_client_ptr, "TrackMe");

  entries_ptr = mongoc_client_get_collection(db_client_ptr, TRACKME_DB,
                                             TRACKME_ENTRIES_COLLECTION);
  if (!entries_ptr) {
    t_log(
        ERROR, __func__,
        "failed get collection [%s] from db [%s]\n" TRACKME_ENTRIES_COLLECTION,
        TRACKME_DB);
    return EXIT_FAILURE;
  }

  mongoc_uri_destroy(m_uri_ptr);
  return EXIT_SUCCESS;
}

int free_db() {
  if (entries_ptr) {
    mongoc_collection_destroy(entries_ptr);
  }

  if (db_client_ptr) {
    mongoc_client_destroy(db_client_ptr);
  }
  mongoc_cleanup();

  return EXIT_SUCCESS;
}

bool save(bson_t *timer_result_ptr) {
  bson_t reply;

  if (!entries_ptr) {
    t_log(ERROR, __func__, "No collection");
    return false;
  }

  bson_error_t error;
  if (!mongoc_collection_insert_one(entries_ptr, timer_result_ptr, NULL, &reply,
                                    &error)) {
    char *bson_as_json =
        bson_as_canonical_extended_json(timer_result_ptr, NULL);
    t_log(ERROR, __func__,
          "failed to insert document: %s\n"
          "error code: [%d]\n"
          "error message: [%s]\n"
          "error reply: [%s]\n",
          bson_as_json, error.code, error.message,
          bson_as_canonical_extended_json(&reply, NULL));
    bson_free(bson_as_json);
    bson_destroy(&reply);
    return false;
  }
  return true;
}

bson_t *get_by_id(int id) { return NULL; }