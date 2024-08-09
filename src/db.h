#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>

#define TRACKME_ENTRIES_COLLECTION "entries"
#define TRACKME_DB "trackme_db"
#define DB_KEY_ID "_id"
#define DB_KEY_NAME "name"
#define DB_KEY_CLIENT "client"
#define DB_KEY_PROJECT "project"
#define DB_KEY_DESCRIPTION "description"
#define DB_KEY_DURATION "duration"
#define DB_KEY_END_TIME "end_time"
#define DB_KEY_START_TIME "start_time"

extern mongoc_collection_t *entries;
extern mongoc_client_t *db_client;


int init_db(const char *);
int free_db(void);
bool save(bson_t *);

#endif // !DB_H
