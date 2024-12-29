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

  t->start_time = -1;
}

void free_timer_result(TimerResult *tr) {
  if (!tr) {
    return;
  }

  if (tr->name) {
    free(tr->name);
  }

  if (tr->client) {
    free(tr->client);
  }

  if (tr->project) {
    free(tr->project);
  }

  if (tr->description) {
    free(tr->description);
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
}

bool start(Timer *t, StartInfo *si) {
  if (!t || !si) {
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
  if (!t) {
    return NULL;
  }

  if (t->start_time < 0) {
    t_log(INFO, __func__, "Timer not started so cannot stop!");
    return NULL;
  }

  TimerResult *tr = malloc(sizeof(TimerResult));
  if (!tr) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  char *name = malloc(strlen(t->name) + 1);
  if (!name) {
    free(tr);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  char *client = malloc(strlen(t->client) + 1);
  if (!client) {
    free(tr);
    free(name);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  char *project = malloc(strlen(t->project) + 1);
  if (!project) {
    free(tr);
    free(name);
    free(client);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  char *description = malloc(strlen(t->description) + 1);
  if (!description) {
    free(tr);
    free(name);
    free(client);
    free(project);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
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
    t_log(ERROR, __func__, "Could not get end time.");
    return NULL;
  }

  tr->duration = tr->end_time - tr->start_time;

  reset(t);

  return tr;
}

char *get_name(Timer *t) {
  if (!t) {
    return NULL;
  }

  if (!t->name) {
    t_log(INFO, __func__, "Name not set, is timer started?");
    return NULL;
  }

  char *name = malloc(strlen(t->name) + 1);
  if (!name) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(name, t->name);
}

char *get_client(Timer *t) {
  if (!t) {
    return NULL;
  }

  if (!t->client) {
    t_log(INFO, __func__, "Client not set, is timer started?");
    return NULL;
  }

  char *client = malloc(strlen(t->client) + 1);
  if (!client) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(client, t->client);
}

char *get_project(Timer *t) {
  if (!t) {
    return NULL;
  }

  if (!t->project) {
    t_log(INFO, __func__, "Project not set, is timer started?");
    return NULL;
  }

  char *project = malloc(strlen(t->project) + 1);
  if (!project) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(project, t->project);
}

char *get_description(Timer *t) {
  if (!t) {
    return NULL;
  }

  if (!t->description) {
    t_log(INFO, __func__, "Description not set, is timer started?");
    return NULL;
  }

  char *description = malloc(strlen(t->description) + 1);
  if (!description) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(description, t->description);
}

int get_duration(Timer *t) {
  if (!t) {
    return 0;
  }

  if (t->start_time < 0) {
    t_log(INFO, __func__, "No start time, is timer started?");
    return 0;
  }

  time_t now = time(NULL);
  if (now <= 0) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return -1;
  }
  if (now - t->start_time < 0) {
    t_log(
        ERROR, __func__,
        "Start time is in the future, negative duration would not make sense!");
    return -1;
  }

  return now - t->start_time;
}
