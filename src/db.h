#ifndef DB_H
#define DB_H

#include "../lib/sqlite3.h"
#include "timer.h"
#include <stdbool.h>

#define TRACKME_ENTRIES_COLLECTION "_entries"
#define TRACKME_DB_FILENAME "./trackme_db"
#define DB_KEY_ID "_id"
#define DB_KEY_ACTIVITY "_activity"
#define DB_KEY_CLIENT "_client"
#define DB_KEY_PROJECT "_project"
#define DB_KEY_DESCRIPTION "_description"
#define DB_KEY_DURATION "_duration"
#define DB_KEY_END_TIME "_end_time"
#define DB_KEY_START_TIME "_start_time"

bool init_db();
bool free_db();
bool save(TimerResult *timer_result);
// bson_t_list *get_by(char *key, void *value);

#endif // !DB_H
