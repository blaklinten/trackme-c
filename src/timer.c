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

  if (t->info.activity) {
    free(t->info.activity);
    t->info.activity = NULL;
  }

  if (t->info.client) {
    free(t->info.client);
    t->info.client = NULL;
  }

  if (t->info.project) {
    free(t->info.project);
    t->info.project = NULL;
  }

  if (t->info.description) {
    free(t->info.description);
    t->info.description = NULL;
  }

  t->start_time = 0;
}

void free_timer_result(TimerResult *tr) {
  if (!tr) {
    return;
  }

  if (tr->info.activity) {
    free(tr->info.activity);
    tr->info.activity = NULL;
  }

  if (tr->info.client) {
    free(tr->info.client);
    tr->info.client = NULL;
  }

  if (tr->info.project) {
    free(tr->info.project);
    tr->info.project = NULL;
  }

  if (tr->info.description) {
    free(tr->info.description);
    tr->info.description = NULL;
  }
  free(tr);
}

void free_start_info(StartInfo *si) {
  if (!si) {
    return;
  }

  if (si->activity) {
    free(si->activity);
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

void free_update_info(UpdateInfo *ui) {
  free_start_info((StartInfo *) ui);
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

  char *activity = malloc(strlen(si->activity) + 1);
  if (!activity) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }
  char *client = malloc(strlen(si->client) + 1);
  if (!client) {
    free(activity);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }
  char *project = malloc(strlen(si->project) + 1);
  if (!project) {
    free(activity);
    free(client);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }
  char *description = malloc(strlen(si->description) + 1);
  if (!description) {
    free(activity);
    free(client);
    free(project);
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return false;
  }

  t->info.activity = strcpy(activity, si->activity);
  t->info.client = strcpy(client, si->client);
  t->info.project = strcpy(project, si->project);
  t->info.description = strcpy(description, si->description);
  free_start_info(si);

  t->start_time = time(NULL);
  if (t->start_time <= 0) {
    reset(t);
    free(activity);
    free(client);
    free(project);
    free(description);
    t_log(ERROR, __func__, "Could not get start time.");
    return false;
  }
  return true;
}

bool update(Timer *t, UpdateInfo *ui) {
  if (!t) {
    t_log(ERROR, __func__, "No timer - cannot update timer!");
    return false;
  }

  if (!ui) {
    t_log(ERROR, __func__, "No updateInfo - cannot update timer!");
    return false;
  }

  if (!t->start_time){
    t_log(ERROR, __func__, "Timer not started so cannot update!");
    return false;
  }

  if (ui->start_time > 0){
    t->start_time = ui->start_time;
  }

  if (ui->end_time > 0){
    // TODO should we stop timer with suggested stop time here?
    t_log(INFO, __func__, "Change stop time of running timer makes no sense.");
  }

  if (ui->info.activity){
    free(t->info.activity);
    char *activity = malloc(strlen(ui->info.activity) + 1);
    if (!activity) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info.activity = strcpy(activity, ui->info.activity);
  }

  if (ui->info.client){
    free(t->info.client);
    char *client = malloc(strlen(ui->info.client) + 1);
    if (!client) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info.client = strcpy(client, ui->info.client);
  }

  if (ui->info.project){
    free(t->info.project);
    char *project = malloc(strlen(ui->info.project) + 1);
    if (!project) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info.project = strcpy(project, ui->info.project);
  }

  if (ui->info.description){
    free(t->info.description);
    char *description = malloc(strlen(ui->info.description) + 1);
    if (!description) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info.description = strcpy(description, ui->info.description);
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
  char *activity = malloc(strlen(t->info.activity) + 1);
  if (!activity) {
    free(tr);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *client = malloc(strlen(t->info.client) + 1);
  if (!client) {
    free(tr);
    free(activity);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *project = malloc(strlen(t->info.project) + 1);
  if (!project) {
    free(tr);
    free(activity);
    free(client);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }
  char *description = malloc(strlen(t->info.description) + 1);
  if (!description) {
    free(tr);
    free(activity);
    free(client);
    free(project);
    tr = NULL;
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    goto finally;
  }

  tr->end_time = time(NULL);
  if (tr->end_time <= 0) {
    free(tr);
    free(activity);
    free(client);
    free(project);
    free(description);
    tr = NULL;
    t_log(ERROR, __func__, "Could not get end time.");
    goto finally;
  }

  tr->info.activity = strcpy(activity, t->info.activity);
  tr->info.client = strcpy(client, t->info.client);
  tr->info.project = strcpy(project, t->info.project);
  tr->info.description = strcpy(description, t->info.description);
  tr->start_time = t->start_time;
  tr->duration = tr->end_time - tr->start_time;

finally:
  reset(t);
  return tr;
}
