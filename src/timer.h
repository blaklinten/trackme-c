#ifndef TIMER
#define TIMER

#include "time.h"
#include <stdlib.h>

enum { TIMER_NOT_STARTED = -1 };

struct StartInfo {
  char *name;
  char *client;
  char *project;
  char *description;
};

struct Timer {
  char *name;
  char *client;
  char *project;
  char *description;
  time_t start_time;
  time_t end_time;
};

struct Timer *create_timer();

time_t start_timer(struct Timer *, struct StartInfo *);

#endif // !TIMER
