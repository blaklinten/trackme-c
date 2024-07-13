#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>

#define TRACKME_ENTRIES_COLLECTION "entries"
#define TRACKME_DB "trackme_db"

extern mongoc_collection_t *entries_ptr;
extern mongoc_client_t *db_client_ptr;

int init_db(const char *);
int free_db(void);

#endif // !DB_H
