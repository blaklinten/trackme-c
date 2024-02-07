#ifndef TIMER_H
#define TIMER_H

#include "time.h"
#include "trackMeError.h"
#include <stdlib.h>

typedef struct StartInfo {
  char *name;
  char *client;
  char *project;
  char *description;
} StartInfo;

typedef struct TimerResult {
  char *name;
  char *client;
  char *project;
  char *description;
  time_t start_time;
  time_t end_time;
  time_t duration;
} TimerResult;

typedef struct Timer {
  char *name;
  char *client;
  char *project;
  char *description;
  time_t start_time;
} Timer;

void reset(Timer *);
ErrorCode start(Timer *, StartInfo *);
ErrorCode stop(Timer *, TimerResult *);
ErrorCode get_duration(Timer *, time_t *);

#endif // !TIMER_H
