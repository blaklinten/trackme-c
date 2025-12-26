#ifndef DB_H
#define DB_H

#include "util/bson_list.h"
#include <mongoc/mongoc.h>

#define TRACKME_ENTRIES_COLLECTION "_entries"
#define TRACKME_DB "trackme_db"
#define DB_KEY_ID "_id"
#define DB_KEY_ACTIVITY "_activity"
#define DB_KEY_CLIENT "_client"
#define DB_KEY_PROJECT "_project"
#define DB_KEY_DESCRIPTION "_description"
#define DB_KEY_DURATION "_duration"
#define DB_KEY_END_TIME "_end_time"
#define DB_KEY_START_TIME "_start_time"

extern mongoc_collection_t *entries;
extern mongoc_client_t *db_client;

int init_db(const char *uri);
int free_db(void);
bool save(bson_t *timer_result);
bson_t_list *get_by(char *key, void *value);

#endif // !DB_H
