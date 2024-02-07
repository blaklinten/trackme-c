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

ErrorCode start(Timer *t, StartInfo *si) {
  assert(t != NULL);
  assert(si != NULL);

  t->name = si->name;
  t->client = si->client;
  t->project = si->project;
  t->description = si->description;
  time(&t->start_time);

  if (t->start_time <= 0) {
    reset(t);
    return E_COULD_NOT_GET_SYSTEM_TIME;
  }

  return E_OK;
}

ErrorCode stop(Timer *t, TimerResult *tr) {
  assert(t != NULL);

  tr->name = t->name;
  tr->client = t->client;
  tr->project = t->project;
  tr->description = t->description;
  tr->start_time = t->start_time;

  time(&tr->end_time);
  if (tr->end_time <= 0) {
    return E_COULD_NOT_GET_SYSTEM_TIME;
  }

  tr->duration = tr->end_time - tr->start_time;

  return E_OK;
}

ErrorCode get_duration(Timer *t, time_t *duration) {
  time_t now = time(NULL);
  if (now <= 0) {
    return E_COULD_NOT_GET_SYSTEM_TIME;
  }
  if (now - t->start_time < 0){
    return E_START_TIME_IN_FUTURE;
  }

  *duration = now - t->start_time;
  return E_OK;
}
