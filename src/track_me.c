#include "track_me.h"
#include "assert.h"
#include "log.h"
#include "timer.h"
#include <stdio.h>
#include <time.h>

char *duration_int_to_string(int i) {
  assert(i);

  int hours = i / 3600;
  int minutes = (i % 3600) / 60;
  int seconds = i % 60;
  int string_size = sizeof("hh:mm:ss");
  char *duration_str = malloc(string_size * sizeof(char));

  if (!duration_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }

  int count = snprintf(duration_str, string_size, "%02d:%02d:%02d", hours,
                       minutes, seconds);
  if (count != string_size - 1) { // -1 exclude terminating \0 byte
    t_log(ERROR, __func__,
          "Did not write correct duration string into buffer, string was [%d] "
          "long with content [%s], should be [%d:%d:%d] (padded to width 2)",
          count, duration_str, hours, minutes, seconds);
    return NULL;
  }
  return duration_str;
}

char *get_current_duration_str(Timer *t) {
  assert(t);

  int duration = get_duration(t);
  if (!duration) {
    return "00:00:00";
  }
  return duration_int_to_string(duration);
}

char *get_start_time_str(TimerResult *tr) {
  assert(tr);

  int string_size = sizeof("DAY, DD MON YYYY HH:MM:SS +ZONE");
  char *start_time_str = malloc(string_size * sizeof(char));
  if (!start_time_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return "Something went wrong :(";
  }
  struct tm *start_time_tm = localtime(&tr->start_time);
  strftime(start_time_str, string_size, "%a, %d %b %Y %H:%M:%S %z",
           start_time_tm);
  return start_time_str;
}

char *get_stop_time_str(TimerResult *tr) {
  assert(tr);

  int string_size = sizeof("DAY, DD MON YYYY HH:MM:SS +ZONE");
  char *end_time_str = malloc(string_size * sizeof(char));
  if (!end_time_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return "Something went wrong :(";
  }
  struct tm *end_time_tm = localtime(&tr->end_time);
  strftime(end_time_str, string_size, "%a, %d %b %Y %H:%M:%S %z", end_time_tm);
  return end_time_str;
}

char *get_final_duration_str(TimerResult *tr) {
  assert(tr);

  int duration = tr->duration;
  return duration_int_to_string(duration);
}
