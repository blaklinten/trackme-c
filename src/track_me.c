#include "track_me.h"
#include "../lib/mongoose.h"
#include "timer.h"
#include "util/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Timer current_timer;
TimerResult *current_timer_result = NULL;

/*** Private helper functions **/
StartInfo *_start_info_from_request_body(LOG_LEVEL log_level_missing_variable,
                                         struct mg_str *request_body) {
  if (!request_body) {
    t_log(ERROR, __func__, "No request_body");
    return NULL;
  }
  // init StartInfo struct
  StartInfo *si = calloc(1, sizeof(StartInfo));
  if (!si) {
    goto log_error;
  }

  char *activity = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!activity) {
    free(si);
    goto log_error;
  }
  char *client = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!client) {
    free(si);
    free(activity);
    goto log_error;
  }
  char *project = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!project) {
    free(si);
    free(activity);
    free(client);
    goto log_error;
  }
  char *description = malloc(REQUEST_FIELD_LONG_SIZE * sizeof(char));
  if (!description) {
    free(si);
    free(activity);
    free(client);
    free(project);
    goto log_error;
  }
  si->activity = activity;
  si->client = client;
  si->project = project;
  si->description = description;

  // Fill StartInfo struct

  char *var_key_activity = "activity";
  if (mg_http_get_var(request_body, var_key_activity, activity,
                      REQUEST_FIELD_SHORT_SIZE) < 1) {
    t_log(log_level_missing_variable, __func__, "Variable [", var_key_activity,
          "] could not be extracted from body. Not set or too "
          "long?");
    strcpy(activity, NOT_SET);
  }

  char *var_key_client = "client";
  if (mg_http_get_var(request_body, var_key_client, client,
                      REQUEST_FIELD_SHORT_SIZE) < 1) {
    t_log(log_level_missing_variable, __func__, "Variable [", var_key_client,
          "] could not be extracted from body. Not set or too "
          "long?");
    strcpy(client, NOT_SET);
  }

  char *var_key_project = "project";
  if (mg_http_get_var(request_body, var_key_project, project,
                      REQUEST_FIELD_SHORT_SIZE) < 1) {
    t_log(log_level_missing_variable, __func__, "Variable [", var_key_project,
          "] could not be extracted from body. Not set or too "
          "long?");
    strcpy(project, NOT_SET);
  }

  char *var_key_description = "description";
  if (mg_http_get_var(request_body, var_key_description, description,
                      REQUEST_FIELD_SHORT_SIZE) < 1) {
    t_log(log_level_missing_variable, __func__, "Variable [",
          var_key_description,
          "] could not be extracted from body. Not set or "
          "too long?");
    strcpy(description, NOT_SET);
  }

  return si;
log_error:
  t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
  return NULL;
};

UpdateInfo *_update_info_from_request_body(struct mg_str *request_body) {

  UpdateInfo *ui = calloc(1, sizeof(UpdateInfo));
  if (!ui) {
    goto log_error;
  }

  StartInfo *info = _start_info_from_request_body(INFO, request_body);
  if (!info) {
    free(ui);
    goto log_error;
  }
  ui->info = info;

  char *start_time_str = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!start_time_str) {
    free(ui);
    free(info);
    goto log_error;
  }

  char *end_time_str = malloc(REQUEST_FIELD_SHORT_SIZE * sizeof(char));
  if (!end_time_str) {
    free(ui);
    free(info);
    free(start_time_str);
    goto log_error;
  }

  char *var_key_start_time = "start_time";
  if (mg_http_get_var(request_body, var_key_start_time, start_time_str,
                      REQUEST_FIELD_SHORT_SIZE) < 1) {
    t_log(INFO, __func__, "Variable [", var_key_start_time,
          "] could not be extracted from body. Not set or "
          "too long?");
    strcpy(start_time_str, "0");
  }
  ui->start_time = atol(start_time_str);
  free(start_time_str);

  char *var_key_end_time = "end_time";
  if (mg_http_get_var(request_body, var_key_end_time, end_time_str,
                      REQUEST_FIELD_SHORT_SIZE) < 1) {
    t_log(INFO, __func__, "Variable [", var_key_end_time,
          "] could not be extracted from body. Not set or "
          "too long?");
    strcpy(end_time_str, "0");
  }
  ui->end_time = atol(end_time_str);
  free(end_time_str);

  return ui;

log_error:
  t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
  return NULL;
}

char *_time_t_to_str(time_t *time) {
  if (!time) {
    t_log(ERROR, __func__, "No time_t to convert");
    char *error_message = malloc(strlen(NULL_TIME) + 1);
    return strcpy(error_message, NULL_TIME);
  }
  size_t string_size = strlen(DATE_FORMAT_STR);
  char *time_str = malloc(string_size * sizeof(char) + 1);
  if (!time_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  struct tm *time_tm = localtime(time);
  if (!time_tm) {
    t_log(ERROR, __func__, "Could not convert time_tm");
    char *error_message = malloc(strlen(NULL_TIME) + 1);
    return strcpy(error_message, NULL_TIME);
  }

  size_t written =
      strftime(time_str, string_size + 1, STRF_DATE_FORMAT, time_tm);
  if (string_size != written) {
    t_log(ERROR, __func__, "Could not fomat date string %s", time_str);
    char *error_message = malloc(strlen(NULL_TIME) + 1);
    return strcpy(error_message, NULL_TIME);
  }
  return time_str;
}

int _get_duration_int() {
  if (current_timer_result) {
    return current_timer_result->duration;
  }

  if (!current_timer.start_time) {
    t_log(ERROR, __func__, "No start time, is timer started?");
    return ERROR_TIMER_NOT_STARTED;
  }

  time_t now = time(NULL);
  if (now <= 0) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return ERROR_MALLOC_FAILURE;
  }

  int duration = now - current_timer.start_time;
  if (duration < 0) {
    t_log(
        ERROR, __func__,
        "Start time is in the future, negative duration would not make sense!");
    return ERROR_NEGATIVE_DURATION;
  }
  return duration;
}

char *_duration_int_to_string(int i) {
  if (i < 0) {
    t_log(ERROR, __func__, "Duration is negative - makes no sense.");
    return NULL;
  }

  int hours = i / 3600;
  int minutes = (i % 3600) / 60;
  int seconds = i % 60;
  int string_size = sizeof("hh:mm:ss");

  char *duration_str = malloc(string_size * sizeof(char));
  if (!duration_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }

  int str_length = snprintf(duration_str, string_size, "%02d:%02d:%02d", hours,
                            minutes, seconds);
  if (str_length != string_size - 1) { // -1 exclude terminating \0 byte
    t_log(ERROR, __func__,
          "Did not write correct duration string into buffer, string was [%d] "
          "long with content [%s], should be [%d:%d:%d] (padded to width 2)",
          str_length, duration_str, hours, minutes, seconds);
    return NULL;
  }
  return duration_str;
}

/*** Public functions ***/

void reset_timer() {
  reset(&current_timer);
  free_timer_result(current_timer_result);
  current_timer_result = NULL;
};

bool start_timer(struct mg_str *request_body) {
  if (is_timer_running()) {
    return false;
  }
  reset_timer();
  return start(&current_timer,
               _start_info_from_request_body(ERROR, request_body));
}

bool update_timer(struct mg_str *request_body) {
  if (!is_timer_running()) {
    return false;
  }
  UpdateInfo *ui =_update_info_from_request_body(request_body); 
  bool updated = update(&current_timer, ui);
  free_update_info(ui);
  return updated;
}

bool is_timer_running() {
  return current_timer_result == NULL && current_timer.start_time;
}

bool stop_timer() { return (current_timer_result = stop(&current_timer)); }

char *get_start_time() {
  if (is_timer_running()) {
    return _time_t_to_str(&current_timer.start_time);
  }

  if (current_timer_result) {
    return _time_t_to_str(&current_timer_result->start_time);
  }
  t_log(ERROR, __func__, "Timer not started nor stopped, no info present.");
  return NULL;
}

char *get_end_time() {
  if (is_timer_running()) {
    t_log(INFO, __func__, "Timer not stopped - no stop_time to get");
    char *timer_not_stopped_str = "No end time - stop timer first";
    char *message = malloc(strlen(timer_not_stopped_str) + 1);
    return strcpy(message, timer_not_stopped_str);
  }

  if (current_timer_result) {
    return _time_t_to_str(&current_timer_result->end_time);
  }

  t_log(ERROR, __func__, "Timer not started nor stopped, no info present.");
  return NULL;
}

char *get_duration() {
  int duration = _get_duration_int();

  if (duration < 0) {
    t_log(ERROR, __func__, "Error when getting duration - aborting.");
    return NULL;
  }
  return _duration_int_to_string(duration);
}

char *get_activity() {
  if (is_timer_running()) {
    if (current_timer.info->activity) {
      char *activity = malloc(strlen(current_timer.info->activity) + 1);
      if (!activity) {
        t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
        return NULL;
      }
      return strcpy(activity, current_timer.info->activity);
    } else {
      t_log(INFO, __func__, "Activity not set");
      char *activity_not_set = malloc(strlen(NOT_SET) + 1);
      return strcpy(activity_not_set, NOT_SET);
    }
  }

  if (current_timer_result) {
    char *activity = malloc(strlen(current_timer_result->info->activity) + 1);
    if (!activity) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return NULL;
    }
    return strcpy(activity, current_timer_result->info->activity);
  }

  t_log(ERROR, __func__, "Timer not started nor stopped, no info present.");
  return NULL;
}

char *get_client() {
  if (is_timer_running()) {
    if (current_timer.info->client) {
      char *client = malloc(strlen(current_timer.info->client) + 1);
      if (!client) {
        t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
        return NULL;
      }
      return strcpy(client, current_timer.info->client);
    } else {
      t_log(INFO, __func__, "Client not set");
      char *client_not_set = malloc(strlen(NOT_SET) + 1);
      return strcpy(client_not_set, NOT_SET);
    }
  }

  if (current_timer_result) {
    char *client = malloc(strlen(current_timer_result->info->client) + 1);
    if (!client) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return NULL;
    }
    return strcpy(client, current_timer_result->info->client);
  }
  t_log(ERROR, __func__, "Timer not started nor stopped, no info present.");
  return NULL;
}

char *get_project() {
  if (is_timer_running()) {
    if (current_timer.info->project) {
      char *project = malloc(strlen(current_timer.info->project) + 1);
      if (!project) {
        t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
        return NULL;
      }
      return strcpy(project, current_timer.info->project);
    } else {
      t_log(INFO, __func__, "Project not set");
      char *client_not_set = malloc(strlen(NOT_SET) + 1);
      return strcpy(client_not_set, NOT_SET);
    }
  }

  if (current_timer_result) {
    char *project = malloc(strlen(current_timer_result->info->project) + 1);
    if (!project) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return NULL;
    }
    return strcpy(project, current_timer_result->info->project);
  }

  t_log(ERROR, __func__, "Timer not started nor stopped, no info present.");
  return NULL;
}

char *get_description() {
  if (is_timer_running()) {
    if (current_timer.info->description) {
      char *description = malloc(strlen(current_timer.info->description) + 1);
      if (!description) {
        t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
        return NULL;
      }
      return strcpy(description, current_timer.info->description);
    } else {
      t_log(INFO, __func__, "Description not set");
      char *client_not_set = malloc(strlen(NOT_SET) + 1);
      return strcpy(client_not_set, NOT_SET);
    }
  }

  if (current_timer_result) {
    char *description =
        malloc(strlen(current_timer_result->info->description) + 1);
    if (!description) {
      t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
      return NULL;
    }
    return strcpy(description, current_timer_result->info->description);
  }

  t_log(ERROR, __func__, "Timer not started nor stopped, no info present.");
  return NULL;
}
