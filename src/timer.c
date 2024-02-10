#include "timer.h"
#include "assert.h"
#include <stdlib.h>

void reset(Timer *t) {
  assert(t != NULL);

  t->name = NULL;
  t->client = NULL;
  t->project = NULL;
  t->description = NULL;
  t->start_time = -1;
}

void start(Timer *t, StartInfo const *si) {
  assert(t != NULL);
  assert(si != NULL);

  t->name = si->name;
  t->client = si->client;
  t->project = si->project;
  t->description = si->description;
  t->start_time = time(NULL);

  if (t->start_time <= 0) {
    reset(t);
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return;
  }
}

TimerResult *stop(Timer const *t) {
  assert(t != NULL);

  tr->name = t->name;
  tr->client = t->client;
  tr->project = t->project;
  tr->description = t->description;
  tr->start_time = t->start_time;

  time(&tr->end_time);
  if (tr->end_time <= 0) {
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return NULL;
  }

  tr->duration = tr->end_time - tr->start_time;

  return tr;
}

int get_duration(Timer const *t) {

  time_t now = time(NULL);
  if (now <= 0) {
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return NULL;
  }
  if (now - t->start_time < 0) {
    error(E_START_TIME_IN_FUTURE);
    return NULL;
  }

  return now - t->start_time;
}
