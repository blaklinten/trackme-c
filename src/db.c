#include "db.h"
#include "../lib/sqlite3.h"
#include "util/log.h"
#include <stdbool.h>

sqlite3 *db;

bool init_db() {
  if (sqlite3_open_v2(TRACKME_DB_FILENAME, &db,
                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                      NULL) != SQLITE_OK) {
    t_log(ERROR, "Could not open db \"" TRACKME_DB_FILENAME "\": ",
          sqlite3_errmsg(db));
    return false;
  }
  // clang-format off
  const char *create_table_sql =
      "CREATE TABLE IF NOT EXISTS " TRACKME_DB_TABLE_TIMER_RESULT " ("
        DB_KEY_ACTIVITY " TEXT, "
        DB_KEY_CLIENT " TEXT, "
        DB_KEY_PROJECT " TEXT, "
        DB_KEY_DESCRIPTION " TEXT, "
        DB_KEY_START_TIME " LONG, "
        DB_KEY_END_TIME " LONG, "
        DB_KEY_DURATION " LONG"
      ");";
  // clang-format on

  char *errMsg;
  if (sqlite3_exec(db, create_table_sql, NULL, NULL, &errMsg) != SQLITE_OK) {
    t_log(ERROR, __func__, "SQL error during table creation: %s\n", errMsg);
    sqlite3_free(errMsg);
  }
  return true;
}

bool free_db() {
  if (sqlite3_close(db) != SQLITE_OK) {
    t_log(ERROR, "Could not close db \"" TRACKME_DB_FILENAME "\": ",
          sqlite3_errmsg(db));
    return false;
  }
  return true;
}

bool save(TimerResult *timer_result) { return true; }

// bson_t *_get_by_id(bson_oid_t id) {
//   bson_t *query = bson_new();
//   BSON_APPEND_OID(query, DB_KEY_ID, &id);
//   mongoc_cursor_t *cursor =
//       mongoc_collection_find_with_opts(entries, query, NULL, NULL);
//   bson_destroy(query);

//   const bson_t *doc;
//   mongoc_cursor_next(cursor, &doc);
//   bson_t *result = bson_copy(doc);
//   mongoc_cursor_destroy(cursor);

//   return result;
// }

// bson_t_list *_get_by_char(char *key, char *value) {
//   bson_t *query = bson_new();
//   BSON_APPEND_UTF8(query, key, value);
//   mongoc_cursor_t *cursor =
//       mongoc_collection_find_with_opts(entries, query, NULL, NULL);
//   bson_destroy(query);

//   const bson_t *doc;
//   bson_t_list *result = create_empty_list();
//   if (!result) {
//     t_log(ERROR, __func__, "Could not create empty list");
//     return NULL;
//   }
//   while (mongoc_cursor_next(cursor, &doc)) {
//     bson_t *current = bson_copy(doc);
//     append_to_list(result, current);
//   }
//   mongoc_cursor_destroy(cursor);
//   return result;
// }

// bson_t_list *_get_by_time(char *key, time_t *value) {
//   bson_t *query = bson_new();
//   BSON_APPEND_TIME_T(query, key, *value);
//   mongoc_cursor_t *cursor =
//       mongoc_collection_find_with_opts(entries, query, NULL, NULL);
//   bson_destroy(query);

//   const bson_t *doc;
//   bson_t_list *result = create_empty_list();
//   if (!result) {
//     t_log(ERROR, __func__, "Could not create empty list");
//     return NULL;
//   }
//   while (mongoc_cursor_next(cursor, &doc)) {
//     bson_t *current = bson_copy(doc);
//     append_to_list(result, current);
//   }
//   mongoc_cursor_destroy(cursor);
//   return result;
// }

// bson_t_list *get_by(char *key, void *value) {
//   if (!key || !value) {
//     t_log(ERROR, __func__, "Key or value is NULL");
//     return NULL;
//   }

//   if (!entries) {
//     t_log(ERROR, __func__, "No collection");
//     return NULL;
//   }

//   if (!strcmp(key, DB_KEY_ID)) {
//     bson_oid_t *id = (bson_oid_t *)value;
//     return create_list_from(_get_by_id(*id));
//   }

//   if (!strcmp(key, DB_KEY_ACTIVITY) || !strcmp(key, DB_KEY_CLIENT) ||
//       !strcmp(key, DB_KEY_PROJECT)) {
//     return _get_by_char(key, value);
//   }

//   if (!strcmp(key, DB_KEY_DURATION) || !strcmp(key, DB_KEY_START_TIME) ||
//       !strcmp(key, DB_KEY_END_TIME)) {
//     time_t *time = (time_t *)value;
//     return _get_by_time(key, time);
//   }
//   t_log(ERROR, __func__, "Key [%s] is not supported", key);
//   return NULL;
// }
