#include "db.h"
#include "../lib/sqlite3.h"
#include "util/log.h"
#include <stdbool.h>

int init_db(const char *uri) { return true; }

int free_db() { return 0; };

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
