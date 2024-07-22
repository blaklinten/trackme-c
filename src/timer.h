#ifndef TIMER_H
#define TIMER_H

#include "time.h"

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
void start(Timer *, StartInfo const *);
TimerResult *stop(Timer const *);
char *get_name(Timer const *);
char *get_client(Timer const *);
char *get_project(Timer const *);
char *get_description(Timer const *);
int get_duration(Timer const *);
time_t get_starttime(Timer const *);

#endif // !TIMER_H
