#include "db.h"
#include "../lib/sqlite3.h"
#include "timer.h"
#include "util/log.h"
#include "util/timer_result_list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 *db = NULL;

/* Private helper functions */

int _db_result_to_list(void *result_list, int number_of_columns,
                       char **contents_of_columns, char **names_of_columns) {
  StartInfo *si = malloc(sizeof(StartInfo));
  if (!si) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return -1;
  }

  TimerResult *result = malloc(sizeof(TimerResult));
  if (!result) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return -1;
  }
  result->info = si;

  for (int i = 0; i < number_of_columns; i++) {
    char *current_column_name = names_of_columns[i];
    char *current_column_content = contents_of_columns[i];

    if (!strcmp(current_column_name, DB_KEY_ACTIVITY)) {
      if (current_column_content) {
        /* This is probably unecessary slow and pedantic. Maybe allocate
        ** fixed size, like REQUEST_FIELD_SHORT_SIZE, instead.
        */
        si->activity = malloc(strlen(current_column_content) + 1);
        strcpy(si->activity, current_column_content);
      } else {
        si->activity = NULL;
      }
      continue;
    } else if (!strcmp(current_column_name, DB_KEY_CLIENT)) {
      if (current_column_content) {
        /* This is probably unecessary slow and pedantic. Maybe allocate
        ** fixed size, like REQUEST_FIELD_SHORT_SIZE, instead.
        */
        si->client = malloc(strlen(current_column_content) + 1);
        strcpy(si->client, current_column_content);
      } else {
        si->client = NULL;
      }
      continue;

    } else if (!strcmp(current_column_name, DB_KEY_PROJECT)) {
      if (current_column_content) {
        /* This is probably unecessary slow and pedantic. Maybe allocate
        ** fixed size, like REQUEST_FIELD_SHORT_SIZE, instead.
        */
        si->project = malloc(strlen(current_column_content) + 1);
        strcpy(si->project, current_column_content);
      } else {
        si->project = NULL;
      }
      continue;

    } else if (!strcmp(current_column_name, DB_KEY_DESCRIPTION)) {
      if (current_column_content) {
        /* This is probably unecessary slow and pedantic. Maybe allocate
        ** fixed size, like REQUEST_FIELD_SHORT_SIZE, instead.
        */
        si->description = malloc(strlen(current_column_content) + 1);
        strcpy(si->description, current_column_content);
      } else {
        si->description = NULL;
      }
      continue;

    } else if (!strcmp(current_column_name, DB_KEY_START_TIME)) {
      if (current_column_content) {
        result->start_time = atoi(current_column_content);
      } else {
        result->start_time = 0;
      }
      continue;

    } else if (!strcmp(current_column_name, DB_KEY_END_TIME)) {
      if (current_column_content) {
        result->end_time = atoi(current_column_content);
      } else {
        result->end_time = 0;
      }
      continue;

    } else if (!strcmp(current_column_name, DB_KEY_DURATION)) {
      if (current_column_content) {
        result->duration = atoi(current_column_content);
      } else {
        result->duration = 0;
      }
      continue;
    }
  }

  if (!append_to_list(result_list, result)) {
    t_log(ERROR, __func__, "Could not add result to list - abort");
    return -1;
  }

  return 0;
}

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
  db = NULL;
  return true;
}

bool save(TimerResult *timer_result) {
  if (!timer_result) {
    t_log(ERROR, __func__, "No timer result to save - abort!");
    return false;
  }
  // clang-format off
  const char *save_result_query =
      "INSERT INTO " TRACKME_DB_TABLE_TIMER_RESULT "("
      DB_KEY_ACTIVITY ", "
      DB_KEY_CLIENT ", "
      DB_KEY_PROJECT ", "
      DB_KEY_DESCRIPTION ", "
      DB_KEY_START_TIME ", "
      DB_KEY_END_TIME ", "
      DB_KEY_DURATION 
      ") VALUES ("
      "?,?,?,?,?,?,?"
      ")";
  // clang-format on
  sqlite3_stmt *insert_stmt;

  if (sqlite3_prepare(db, save_result_query, -1, &insert_stmt, NULL) !=
      SQLITE_OK) {
    t_log(ERROR, __func__, "Could not prepare insert statement: %s",
          sqlite3_errmsg(db));
    return false;
  }

  if (sqlite3_bind_text(insert_stmt, 1, timer_result->info->activity, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 2, timer_result->info->client, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 3, timer_result->info->project, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_text(insert_stmt, 4, timer_result->info->description, -1,
                        SQLITE_STATIC) != SQLITE_OK ||
      sqlite3_bind_int(insert_stmt, 5, timer_result->start_time) != SQLITE_OK ||
      sqlite3_bind_int(insert_stmt, 6, timer_result->end_time) != SQLITE_OK ||
      sqlite3_bind_int(insert_stmt, 7, timer_result->duration) != SQLITE_OK) {
    t_log(ERROR, __func__, "Could not bind parameters to insert statement: %s",
          sqlite3_errmsg(db));
    return false;
  };

  if (sqlite3_step(insert_stmt) != SQLITE_DONE) {
    t_log(ERROR, __func__, "Could not execute insert statement: %s",
          sqlite3_errmsg(db));
    return false;
  }

  sqlite3_finalize(insert_stmt);
  return true;
}

// TimerResult *_get_by_id(bson_oid_t id) {
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
// return NULL;
// }

timer_result_list *_get_by_char(char *key, char *value) {
  const char *query_format =
      "SELECT * FROM " TRACKME_DB_TABLE_TIMER_RESULT " WHERE  %s IS '%s'";
  int query_length = strlen(query_format) + strlen(key) + strlen(value);
  char *query = malloc(query_length + 1); // + 1 = \0 ?
  snprintf(query, query_length, query_format, key, value);

  timer_result_list *result = create_empty_list();
  char *errmsg = NULL;
  if (sqlite3_exec(db, query, _db_result_to_list, result, &errmsg) ==
      SQLITE_ABORT) {
    t_log(ERROR, __func__,
          "Callback returned non-zero - could not create result list");
    free(query);
    free_list(result);
    sqlite3_free(errmsg);
    return NULL;
  }

  if (errmsg != NULL) {
    t_log(ERROR, __func__, "Could not parse SQL-statement: %s", errmsg);
    free(query);
    free_list(result);
    sqlite3_free(errmsg);
    return NULL;
  }

  free(query);
  return result;
}

timer_result_list *_get_by_time(char *key, time_t *value) { return NULL; }

timer_result_list *get_by(char *key, void *value) {
  if (!key || !value) {
    t_log(ERROR, __func__, "Key or value is NULL");
    return NULL;
  }

  if (!db) {
    t_log(ERROR, __func__, "No open db");
    return NULL;
  }

  // TODO the DB should have a ID field
  // if (!strcmp(key, DB_KEY_ID)) {
  //   bson_oid_t *id = (bson_oid_t *)value;
  //   return create_list_from(_get_by_id(*id));
  // }

  // strcmp -> 0 if eq
  if (!(strcmp(key, DB_KEY_ACTIVITY) && strcmp(key, DB_KEY_CLIENT) &&
        strcmp(key, DB_KEY_PROJECT))) {
    return _get_by_char(key, value);
  }

  if (!(strcmp(key, DB_KEY_DURATION) && strcmp(key, DB_KEY_START_TIME) &&
        strcmp(key, DB_KEY_END_TIME))) {
    time_t *time = (time_t *)value;
    return _get_by_time(key, time);
  }
  t_log(ERROR, __func__, "Key [%s] is not supported", key);
  return NULL;
}
