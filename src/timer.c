#include "timer.h"
#include "util/log.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

// TODO write test for this
StartInfo *copy_start_info(StartInfo *orig_si) {
  if (!orig_si) {
    t_log(ERROR, __func__, "No StartInfo to copy!");
  }
  // init StartInfo struct
  StartInfo *si = calloc(1, sizeof(StartInfo));
  if (!si) {
  }

  char *activity = malloc(strlen(orig_si->activity) + 1);
  if (!activity) {
    free(si);
  }
  char *client = malloc(strlen(orig_si->client) + 1);
  if (!client) {
    free(si);
    free(activity);
  }
  char *project = malloc(strlen(orig_si->project) + 1);
  if (!project) {
    free(si);
    free(activity);
    free(client);
  }
  char *description = malloc(strlen(orig_si->description) + 1);
  if (!description) {
    free(si);
    free(activity);
    free(client);
    free(project);
  }

  // Fill new StartInfo struct
  si->activity = strcpy(activity, orig_si->activity);
  si->client = strcpy(client, orig_si->client);
  si->project = strcpy(project, orig_si->project);
  si->description = strcpy(description, orig_si->description);

  return si;
};

void reset(Timer *t) {
  if (!t) {
    t_log(DEBUG, __func__, "Timer is NULL, something is wrong here...");
    return;
  }

  free_start_info(t->info);
  t->info = NULL;
  t->start_time = 0;
}

void free_timer_result(TimerResult *tr) {
  if (!tr) {
    t_log(DEBUG, __func__, "TimerResult is NULL, nothing to free...");
    return;
  }

  free_start_info(tr->info);

  free(tr);
}

void free_update_info(UpdateInfo *ui) {
  if (!ui) {
    t_log(DEBUG, __func__, "TimerResult is NULL, nothing to free...");
    return;
  }

  free_start_info(ui->info);
  
  free(ui);
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

  t->info = si;
  t->start_time = time(NULL);
  if (t->start_time <= 0) {
    reset(t);
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

  if (ui->info->activity){
    free(t->info->activity);
    char *activity = malloc(strlen(ui->info->activity) + 1);
    if (!activity) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info->activity = strcpy(activity, ui->info->activity);
  }

  if (ui->info->client){
    free(t->info->client);
    char *client = malloc(strlen(ui->info->client) + 1);
    if (!client) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info->client = strcpy(client, ui->info->client);
  }

  if (ui->info->project){
    free(t->info->project);
    char *project = malloc(strlen(ui->info->project) + 1);
    if (!project) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info->project = strcpy(project, ui->info->project);
  }

  if (ui->info->description){
    free(t->info->description);
    char *description = malloc(strlen(ui->info->description) + 1);
    if (!description) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return false;
    }
    t->info->description = strcpy(description, ui->info->description);
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

  tr->info = copy_start_info(t->info);
  tr->end_time = time(NULL);
  if (tr->end_time <= 0) {
    free(tr);
    tr = NULL;
    t_log(ERROR, __func__, "Could not get end time.");
    goto finally;
  }

  tr->start_time = t->start_time;
  tr->duration = tr->end_time - tr->start_time;

finally:
  reset(t);
  return tr;
}
