#ifndef TIMER_H
#define TIMER_H

#include "time.h"
#include <stdbool.h>

typedef struct {
  char *activity;
  char *client;
  char *project;
  char *description;
} StartInfo;

typedef struct {
  StartInfo info;
  time_t start_time;
} Timer;

typedef struct {
  StartInfo info;
  time_t start_time;
  time_t end_time;
} UpdateInfo;

typedef struct {
  StartInfo info;
  time_t start_time;
  time_t end_time;
  time_t duration;
} TimerResult;

void reset(Timer *t);
void free_timer_result(TimerResult *tr);
void free_start_info(StartInfo *si);
void free_update_info(UpdateInfo *ui);
bool start(Timer *t, StartInfo *si);
bool update(Timer *t, UpdateInfo *ui);
TimerResult *stop(Timer *t);

#endif // !TIMER_H
