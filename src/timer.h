#ifndef TIMER_H
#define TIMER_H

#include "time.h"
#include <stdbool.h>

typedef struct {
  char *name;
  char *client;
  char *project;
  char *description;
} StartInfo;

typedef struct {
  char *name;
  char *client;
  char *project;
  char *description;
  time_t start_time;
  time_t end_time;
  int duration;
} TimerResult;

typedef struct {
  char *name;
  char *client;
  char *project;
  char *description;
  time_t start_time;
} Timer;

bool start(Timer *t, StartInfo *si);
TimerResult *stop(Timer *t);
void reset(Timer *t);
void free_timer_result(TimerResult *tr);
void free_start_info(StartInfo *si);

#endif // !TIMER_H
