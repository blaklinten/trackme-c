#ifndef TRACK_ME
#define TRACK_ME

#include "timer.h"
#include <bson/bson.h>

bson_t *from_timer_result(TimerResult *);
TimerResult *from_bson(bson_t *);
char *get_current_duration_str(Timer *);
char *get_final_duration_str(TimerResult *);
char *get_start_time_str(TimerResult *);
char *get_stop_time_str(TimerResult *);

#endif // !TRACK_ME
