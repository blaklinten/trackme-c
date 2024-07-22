#ifndef TRACK_ME
#define TRACK_ME

#include "timer.h"
#include <bson/bson.h>

#define DB_KEY_NAME "name"
#define DB_KEY_CLIENT "client"
#define DB_KEY_PROJECT "project"
#define DB_KEY_DESCRIPTION "description"
#define DB_KEY_DURATION "duration"
#define DB_KEY_END_TIME "end_time"
#define DB_KEY_START_TIME "start_time"

bson_t *from_timer_result(TimerResult *);
TimerResult *from_bson(bson_t *);

#endif // !TRACK_ME
