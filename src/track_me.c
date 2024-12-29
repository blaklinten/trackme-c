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

// TODO implement these -->
bson_t *from_timer_result(TimerResult *tr) {
  (void)tr;
  return bson_new();
}
TimerResult *from_bson(bson_t *b) {
  (void)b;
  return malloc(sizeof(TimerResult));
}
// TODO <--

/*** Private helper functions **/
StartInfo *_start_info_from_request_body(struct mg_str *request_body) {
  if (!request_body) {
    t_log(ERROR, __func__, "No request_body");
    return NULL;
  }
  // init StartInfo struct
  StartInfo *si = calloc(1, sizeof(StartInfo));
  if (!si) {
    goto log_error;
  }

  char *name = malloc(REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!name) {
    free(si);
    goto log_error;
  }
  char *client = malloc(REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!client) {
    free(si);
    free(name);
    goto log_error;
  }
  char *project = malloc(REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!project) {
    free(si);
    free(name);
    free(client);
    goto log_error;
  }
  char *description = malloc(10 * REQUEST_FIELD_MAX_SIZE * sizeof(char));
  if (!description) {
    free(si);
    free(name);
    free(client);
    free(project);
    goto log_error;
  }
  si->name = name;
  si->client = client;
  si->project = project;
  si->description = description;

  // Fill StartInfo struct

  char *var_key_name = "name";
  if (mg_http_get_var(request_body, var_key_name, name,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [name] could not be extracted from body. Not set or too "
          "long?");
    snprintf(name, NOT_SET_LENGTH, "%s", NOT_SET);
  }

  char *var_key_client = "client";
  if (mg_http_get_var(request_body, var_key_client, client,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [client] could not be extracted from body. Not set or too "
          "long?");
    snprintf(client, NOT_SET_LENGTH, "%s", NOT_SET);
  }

  char *var_key_project = "project";
  if (mg_http_get_var(request_body, var_key_project, project,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [project] could not be extracted from body. Not set or too "
          "long?");
    snprintf(project, NOT_SET_LENGTH, "%s", NOT_SET);
  }

  char *var_key_description = "description";
  if (mg_http_get_var(request_body, var_key_description, description,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [description] could not be extracted from body. Not set or "
          "too long?");
    snprintf(description, NOT_SET_LENGTH, "%s", NOT_SET);
  }

  return si;
log_error:
  t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
  return NULL;
};

char *_time_t_to_str(time_t *time) {
  if (!time) {
    t_log(ERROR, __func__, "No time_t to convert");
    char *error_message = malloc(strlen(NULL_TIME) + 1);
    snprintf(error_message, strlen(NULL_TIME) + 1, "%s", NULL_TIME);
    return error_message;
  }
  size_t string_size = strlen(DATE_FORMAT_STR);
  char *time_str = malloc(string_size * sizeof(char) + 1);
  if (!time_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  struct tm *time_tm = localtime(time);
  size_t written =
      strftime(time_str, string_size + 1, STRF_DATE_FORMAT, time_tm);
  if (string_size != written) {
    t_log(ERROR, __func__, "Could not fomat date string %s", time_str);
    char *error_message = malloc(strlen(NULL_TIME) + 1);
    snprintf(error_message, strlen(NULL_TIME) + 1, "%s", NULL_TIME);
    return error_message;
  }
  return time_str;
}

int _get_duration_int() {
  if (current_timer_result) {
    return current_timer_result->duration;
  }

  if (!current_timer.start_time) {
    t_log(INFO, __func__, "No start time, is timer started?");
    return 0;
  }

  time_t now = time(NULL);
  if (now <= 0) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return -1;
  }

  int duration = now - current_timer.start_time;
  if (duration < 0) {
    t_log(
        ERROR, __func__,
        "Start time is in the future, negative duration would not make sense!");
  }
  return duration;
}

char *_duration_int_to_string(int i) {
  if (i < 0){
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

/*** Public funcions ***/

bool start_timer(struct mg_str *request_body) {
  reset(&current_timer);
  free_timer_result(current_timer_result);
  current_timer_result = NULL;
  return start(&current_timer, _start_info_from_request_body(request_body));
}

bool is_timer_running() {
  return current_timer_result == NULL && current_timer.start_time;
}

bool stop_timer() {
  return (current_timer_result = stop(&current_timer));
}

char *get_start_time() {
  if (current_timer_result) {
    return _time_t_to_str(&current_timer_result->start_time);
  } else {
    return _time_t_to_str(&current_timer.start_time);
  }
}

char *get_end_time() {
  if (current_timer_result) {
    return _time_t_to_str(&current_timer_result->end_time);
  } else {
    t_log(INFO, __func__, "Timer not stopped - no stop_time to get");
    char *timer_not_stopped_str_length = "No end time - stop timer first";
    char *timer_not_stopped = malloc(strlen(timer_not_stopped_str_length) + 1);
    snprintf(timer_not_stopped, strlen(timer_not_stopped_str_length) + 1, "%s",
             timer_not_stopped_str_length);
    return timer_not_stopped;
  }
}

char *get_duration() {
  int duration = _get_duration_int();
  if (duration < 1) {
    char *no_duration_str = malloc(strlen(ZERO_STR) + 1);
    snprintf(no_duration_str, strlen(ZERO_STR) + 1, "%s", ZERO_STR);
    return no_duration_str;
  }
  return _duration_int_to_string(duration);
}

char *get_name() {
  if (current_timer_result) {
    return current_timer_result->name;
  }

  if (!current_timer.name) {
    t_log(INFO, __func__, "Name not set");
    char *name_not_set = malloc(strlen(NOT_SET) + 1);
    snprintf(name_not_set, strlen(NOT_SET) + 1, "%s", NOT_SET);
    return name_not_set;
  }

  char *name = malloc(strlen(current_timer.name) + 1);
  if (!name) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(name, current_timer.name);
}

char *get_client() {
  if (current_timer_result) {
    return current_timer_result->client;
  }

  if (!current_timer.client) {
    t_log(INFO, __func__, "Client not set");
    char *client_not_set = malloc(strlen(NOT_SET) + 1);
    snprintf(client_not_set, strlen(NOT_SET) + 1, "%s", NOT_SET);
    return client_not_set;
  }

  char *client = malloc(strlen(current_timer.client) + 1);
  if (!client) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(client, current_timer.client);
}

char *get_project() {
  if (current_timer_result) {
    return current_timer_result->project;
  }

  if (!current_timer.project) {
    t_log(INFO, __func__, "Project not set");
    char *project_not_set = malloc(strlen(NOT_SET) + 1);
    snprintf(project_not_set, strlen(NOT_SET) + 1, "%s", NOT_SET);
    return project_not_set;
  }

  char *project = malloc(strlen(current_timer.project) + 1);
  if (!project) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(project, current_timer.project);
}
char *get_description() {
  if (current_timer_result) {
    return current_timer_result->description;
  }

  if (!current_timer.description) {
    t_log(INFO, __func__, "Description not set");
    char *description_not_set = malloc(strlen(NOT_SET) + 1);
    snprintf(description_not_set, strlen(NOT_SET) + 1, "%s", NOT_SET);
    return description_not_set;
  }

  char *description = malloc(strlen(current_timer.description) + 1);
  if (!description) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  return strcpy(description, current_timer.description);
}
