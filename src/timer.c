#include "timer.h"
#include "assert.h"
#include <stdlib.h>
#include <string.h>

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

  char *name = malloc(strlen(si->name) + 1);
  if (!name) {
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return;
  }
  char *client = malloc(strlen(si->client) + 1);
  if (!client) {
    free(name);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return;
  }
  char *project = malloc(strlen(si->project) + 1);
  if (!project) {
    free(name);
    free(client);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return;
  }
  char *description = malloc(strlen(si->description) + 1);
  if (!description) {
    free(name);
    free(client);
    free(project);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return;
  }

  t->name = strcpy(name, si->name);
  t->client = strcpy(client, si->client);
  t->project = strcpy(project, si->project);
  t->description = strcpy(description, si->description);
  t->start_time = time(NULL);

  if (t->start_time <= 0) {
    reset(t);
    free(name);
    free(client);
    free(project);
    free(description);
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return;
  }
}

TimerResult *stop(Timer const *t) {
  assert(t != NULL);
  TimerResult *tr = malloc(sizeof(TimerResult));
  if (!tr) {
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return NULL;
  }
  char *name = malloc(strlen(t->name) + 1);
  if (!name) {
    free(tr);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  char *client = malloc(strlen(t->client) + 1);
  if (!client) {
    free(tr);
    free(name);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  char *project = malloc(strlen(t->project) + 1);
  if (!project) {
    free(tr);
    free(name);
    free(client);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  char *description = malloc(strlen(t->description) + 1);
  if (!description) {
    free(tr);
    free(name);
    free(client);
    free(project);
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }

  tr->name = strcpy(name, t->name);
  tr->client = strcpy(client, t->client);
  tr->project = strcpy(project, t->project);
  tr->description = strcpy(description, t->description);
  tr->start_time = t->start_time;
  tr->end_time = time(NULL);
  if (tr->end_time <= 0) {
    free(tr);
    free(name);
    free(client);
    free(project);
    free(description);
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return NULL;
  }

  tr->duration = tr->end_time - tr->start_time;

  return tr;
}

char *get_name(Timer const *t) {
  assert(t != NULL);

  if (!t->name) {
    error(E_TIMER_NOT_STARTED);
    return NULL;
  }

  char *name = malloc(strlen(t->name) + 1);
  if (!name){
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  return strcpy(name, t->name);
}

char *get_client(Timer const *t) {
  assert(t != NULL);

  if (!t->client) {
    error(E_TIMER_NOT_STARTED);
    return NULL;
  }

  char *client = malloc(strlen(t->client) + 1);
  if (!client){
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  return strcpy(client, t->client);
}

char *get_project(Timer const *t) {
  assert(t != NULL);

  if (!t->project) {
    error(E_TIMER_NOT_STARTED);
    return NULL;
  }

  char *project = malloc(strlen(t->project) + 1);
  if (!project){
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  return strcpy(project, t->project);
}

char *get_description(Timer const *t) {
  assert(t != NULL);

  if (!t->description) {
    error(E_TIMER_NOT_STARTED);
    return NULL;
  }

  char *description = malloc(strlen(t->description) + 1);
  if (!description){
    error(E_COULD_NOT_ALLOCATE_ENOUGH_MEMORY);
    return NULL;
  }
  return strcpy(description, t->description);
}

int get_duration(Timer const *t) {
  assert(t != NULL);

  time_t now = time(NULL);
  if (now <= 0) {
    error(E_COULD_NOT_GET_SYSTEM_TIME);
    return 0;
  }
  if (now - t->start_time < 0) {
    error(E_START_TIME_IN_FUTURE);
    return 0;
  }

  return now - t->start_time;
}
