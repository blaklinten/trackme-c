#ifndef TRACK_ME
#define TRACK_ME

#include "../lib/mongoose.h"
#include "timer.h"
#include <bson/bson.h>

#define REQUEST_FIELD_MAX_SIZE 50

bson_t *from_timer_result(TimerResult *);
TimerResult *from_bson(bson_t *);
char *get_current_duration_str(Timer *);
char *get_final_duration_str(TimerResult *);
char *get_start_time_str(TimerResult *);
char *get_stop_time_str(TimerResult *);
StartInfo *from_request_body(struct mg_str *);

#endif // !TRACK_ME
