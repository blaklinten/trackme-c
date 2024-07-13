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
