#ifndef DB_H
#define DB_H

#include <mongoc/mongoc.h>

#define TRACKME_ENTRIES_COLLECTION "entries"
#define TRACKME_DB "trackme_db"
#define DB_KEY_ID "_id"

extern mongoc_collection_t *entries;
extern mongoc_client_t *db_client;

typedef struct bson_t_list {
  bson_t *value;
  struct bson_t_list *next;
  struct bson_t_list *previous;
} bson_t_list;

int init_db(const char *);
int free_db(void);
bool save(bson_t *);
bson_t *get_by_id(bson_oid_t);
bson_t_list *get_by(char *, void *);
void free_list(bson_t_list *);
int count_elements(bson_t_list *);

#endif // !DB_H
