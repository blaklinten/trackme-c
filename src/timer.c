#include "timer.h"
#include "util/log.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void reset(Timer *t) {
  if (!t) {
    t_log(INFO, __func__, "Timer is NULL, something is wrong here...");
    return;
  }

  if (t->name) {
    free(t->name);
    t->name = NULL;
  }

  if (t->client) {
    free(t->client);
    t->client = NULL;
  }

  if (t->project) {
    free(t->project);
    t->project = NULL;
  }

  if (t->description) {
    free(t->description);
    t->description = NULL;
  }

  t->start_time = 0;
}

void free_timer_result(TimerResult *tr) {
  if (!tr) {
    return;
  }

  if (tr->name) {
    free(tr->name);
    tr->name = NULL;
  }

  if (tr->client) {
    free(tr->client);
    tr->client = NULL;
  }

  if (tr->project) {
    free(tr->project);
    tr->project = NULL;
  }

  if (tr->description) {
    free(tr->description);
    tr->description = NULL;
  }
  free(tr);
}

void free_start_info(StartInfo *si) {
  if (!si) {
    return;
  }

  if (si->name) {
    free(si->name);
  }

  if (si->client) {
    free(si->client);
  }

  if (si->project) {
    free(si->project);
  }

  if (si->description) {
    free(si->description);
  }

  free(si);
  si = NULL;
}

bool start(Timer *t, StartInfo *si) {
  if (!t) {
    t_log(ERROR, __func__, "No timer - cannot start timer!");
    return false;
  }

  if (!si) {
    t_log(ERROR, __func__, "No startinfo - cannot start timer!");
    return false;
  }

  if (t->start_time > 0) {
    t_log(INFO, __func__, "Timer already started, won't start agin.");
    return false;
  }

  char *name = malloc(strlen(si->name) + 1);
  if (!name) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }
  char *client = malloc(strlen(si->client) + 1);
  if (!client) {
    free(name);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }
  char *project = malloc(strlen(si->project) + 1);
  if (!project) {
    free(name);
    free(client);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }
  char *description = malloc(strlen(si->description) + 1);
  if (!description) {
    free(name);
    free(client);
    free(project);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }

  t->name = strcpy(name, si->name);
  t->client = strcpy(client, si->client);
  t->project = strcpy(project, si->project);
  t->description = strcpy(description, si->description);
  free_start_info(si);

  t->start_time = time(NULL);
  if (t->start_time <= 0) {
    reset(t);
    free(name);
    free(client);
    free(project);
    free(description);
    t_log(ERROR, __func__, "Could not get start time.");
    return false;
  }
  return true;
}

TimerResult *stop(Timer *t) {
  TimerResult *tr = NULL;

  if (!t) {
    t_log(ERROR, __func__, "No timer, cannot stop timer!");
    goto finally;
  }

  if (!t->start_time) {
    t_log(INFO, __func__, "Timer not started so cannot stop!");
    goto finally;
  }

  tr = malloc(sizeof(TimerResult));
  if (!tr) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *name = malloc(strlen(t->name) + 1);
  if (!name) {
    free(tr);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *client = malloc(strlen(t->client) + 1);
  if (!client) {
    free(tr);
    free(name);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *project = malloc(strlen(t->project) + 1);
  if (!project) {
    free(tr);
    free(name);
    free(client);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *description = malloc(strlen(t->description) + 1);
  if (!description) {
    free(tr);
    free(name);
    free(client);
    free(project);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }

  tr->end_time = time(NULL);
  if (tr->end_time <= 0) {
    free(tr);
    free(name);
    free(client);
    free(project);
    free(description);
    tr = NULL;
    t_log(ERROR, __func__, "Could not get end time.");
    goto finally;
  }

  tr->name = strcpy(name, t->name);
  tr->client = strcpy(client, t->client);
  tr->project = strcpy(project, t->project);
  tr->description = strcpy(description, t->description);
  tr->start_time = t->start_time;
  tr->duration = tr->end_time - tr->start_time;

finally:
  reset(t);
  return tr;
}
