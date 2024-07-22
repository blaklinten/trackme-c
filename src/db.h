#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>

#define TRACKME_ENTRIES_COLLECTION "entries"
#define TRACKME_DB "trackme_db"

extern mongoc_collection_t *entries;
extern mongoc_client_t *db_client;

int init_db(const char *);
int free_db(void);
bool save(bson_t *);
bson_t *get_by_id(int);

#endif // !DB_H
