#include "db.h"
#include "log.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>

mongoc_collection_t *entries = NULL;
mongoc_client_t *db_client = NULL;

int init_db(const char *uri) {

  mongoc_init();
  bson_error_t error;
  mongoc_uri_t *m_uri = mongoc_uri_new_with_error(uri, &error);
  if (!m_uri) {
    t_log(ERROR, __func__,
          "failed to parse URI: %s\n"
          "error message:       %s\n",
          uri, error.message);
    return EXIT_FAILURE;
  }
  db_client = mongoc_client_new_from_uri(m_uri);
  if (!db_client) {
    fprintf(stderr, "failed to get db_client \n");
    return EXIT_FAILURE;
  }
  mongoc_client_set_appname(db_client, "TrackMe");

  entries = mongoc_client_get_collection(db_client, TRACKME_DB,
                                         TRACKME_ENTRIES_COLLECTION);
  if (!entries) {
    t_log(
        ERROR, __func__,
        "failed get collection [%s] from db [%s]\n" TRACKME_ENTRIES_COLLECTION,
        TRACKME_DB);
    return EXIT_FAILURE;
  }

  mongoc_uri_destroy(m_uri);
  return EXIT_SUCCESS;
}

int free_db() {
  if (entries) {
    mongoc_collection_destroy(entries);
  }

  if (db_client) {
    mongoc_client_destroy(db_client);
  }
  mongoc_cleanup();

  return EXIT_SUCCESS;
}

bool save(bson_t *timer_result) {

  if (!entries) {
    t_log(ERROR, __func__, "No collection");
    return false;
  }

  bson_t reply;
  bson_error_t error;
  if (!mongoc_collection_insert_one(entries, timer_result, NULL, &reply,
                                    &error)) {
    char *bson_as_json = bson_as_canonical_extended_json(timer_result, NULL);
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
