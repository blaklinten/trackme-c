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

void reset(Timer *);
void free_timer_result(TimerResult *);
bool start(Timer *, StartInfo *);
TimerResult *stop(Timer *);
char *get_name(Timer *);
char *get_client(Timer *);
char *get_project(Timer *);
char *get_description(Timer *);
int get_duration(Timer *);
time_t get_starttime(Timer *);

#endif // !TIMER_H
