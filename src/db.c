#include "db.h"
#include "../lib/sqlite3.h"
#include "util/bson_list.h"
#include "util/log.h"
#include <bson/bson.h>
#include <string.h>

mongoc_collection_t *entries = NULL;
mongoc_client_t *db_client = NULL;

int init_db(const char *uri) {
  if (!uri) {
    t_log(ERROR, __func__, "uri is NULL, init failed");
    return -1;
  }

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
  if (!timer_result) {
    t_log(ERROR, __func__, "Timer result is NULL");
    return false;
  }
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

bson_t *_get_by_id(bson_oid_t id) {
  bson_t *query = bson_new();
  BSON_APPEND_OID(query, DB_KEY_ID, &id);
  mongoc_cursor_t *cursor =
      mongoc_collection_find_with_opts(entries, query, NULL, NULL);
  bson_destroy(query);

  const bson_t *doc;
  mongoc_cursor_next(cursor, &doc);
  bson_t *result = bson_copy(doc);
  mongoc_cursor_destroy(cursor);

  return result;
}

bson_t_list *_get_by_char(char *key, char *value) {
  bson_t *query = bson_new();
  BSON_APPEND_UTF8(query, key, value);
  mongoc_cursor_t *cursor =
      mongoc_collection_find_with_opts(entries, query, NULL, NULL);
  bson_destroy(query);

  const bson_t *doc;
  bson_t_list *result = create_empty_list();
  if (!result) {
    t_log(ERROR, __func__, "Could not create empty list");
    return NULL;
  }
  while (mongoc_cursor_next(cursor, &doc)) {
    bson_t *current = bson_copy(doc);
    append_to_list(result, current);
  }
  mongoc_cursor_destroy(cursor);
  return result;
}

bson_t_list *_get_by_time(char *key, time_t *value) {
  bson_t *query = bson_new();
  BSON_APPEND_TIME_T(query, key, *value);
  mongoc_cursor_t *cursor =
      mongoc_collection_find_with_opts(entries, query, NULL, NULL);
  bson_destroy(query);

  const bson_t *doc;
  bson_t_list *result = create_empty_list();
  if (!result) {
    t_log(ERROR, __func__, "Could not create empty list");
    return NULL;
  }
  while (mongoc_cursor_next(cursor, &doc)) {
    bson_t *current = bson_copy(doc);
    append_to_list(result, current);
  }
  mongoc_cursor_destroy(cursor);
  return result;
}

bson_t_list *get_by(char *key, void *value) {
  if (!key || !value) {
    t_log(ERROR, __func__, "Key or value is NULL");
    return NULL;
  }

  if (!entries) {
    t_log(ERROR, __func__, "No collection");
    return NULL;
  }

  if (!strcmp(key, DB_KEY_ID)) {
    bson_oid_t *id = (bson_oid_t *)value;
    return create_list_from(_get_by_id(*id));
  }

  if (!strcmp(key, DB_KEY_ACTIVITY) || !strcmp(key, DB_KEY_CLIENT) ||
      !strcmp(key, DB_KEY_PROJECT)) {
    return _get_by_char(key, value);
  }

  if (!strcmp(key, DB_KEY_DURATION) || !strcmp(key, DB_KEY_START_TIME) ||
      !strcmp(key, DB_KEY_END_TIME)) {
    time_t *time = (time_t *)value;
    return _get_by_time(key, time);
  }
  t_log(ERROR, __func__, "Key [%s] is not supported", key);
  return NULL;
}
