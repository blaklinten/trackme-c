#ifndef TRACK_ME
#define TRACK_ME

#include "../lib/mongoose.h"
#include <bson/bson.h>

#define REQUEST_FIELD_SHORT_SIZE 50
#define REQUEST_FIELD_LONG_SIZE 500
#define DATE_FORMAT_STR "DAY, DD MON YYYY HH:MM:SS +ZONE"
#define STRF_DATE_FORMAT "%a, %d %b %Y %H:%M:%S %z"
#define NULL_TIME "Thu, 01 Jan 1970 01:00:00 +0100"
#define ZERO_STR  "00:00:00"
#define NOT_SET "Not set"
#define NOT_SET_LENGTH (strlen(NOT_SET) + 1)

enum TRACKME_ERROR {
  ERROR_TIMER_NOT_STARTED = -1,
  ERROR_NEGATIVE_DURATION = -2,
  ERROR_MALLOC_FAILURE = -3
};

bool start_timer(struct mg_str *request_body);
bool update_timer(struct mg_str *request_body);
bool is_timer_running();
bool stop_timer();
char *get_start_time();
char *get_end_time();
char *get_duration();
char *get_activity();
char *get_client();
char *get_project();
char *get_description();

#endif // !TRACK_ME
