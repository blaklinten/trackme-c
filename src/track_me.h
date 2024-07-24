#ifndef TRACK_ME
#define TRACK_ME

#include "timer.h"
#include <bson/bson.h>

bson_t *from_timer_result(TimerResult *);
TimerResult *from_bson(bson_t *);

#endif // !TRACK_ME
