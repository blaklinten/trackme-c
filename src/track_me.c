#include "track_me.h"
#include "../lib/mongoose.h"
#include "timer.h"
#include "util/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bson_t *from_timer_result(TimerResult *t) {
  (void)t;
  return bson_new();
}
TimerResult *from_bson(bson_t *b) {
  (void)b;
  return malloc(sizeof(TimerResult));
}

char *_duration_int_to_string(int i) {
  int hours = i / 3600;
  int minutes = (i % 3600) / 60;
  int seconds = i % 60;
  int string_size = sizeof("hh:mm:ss");
  char *duration_str = malloc(string_size * sizeof(char));

  if (!duration_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }

  int count = snprintf(duration_str, string_size, "%02d:%02d:%02d", hours,
                       minutes, seconds);
  if (count != string_size - 1) { // -1 exclude terminating \0 byte
    t_log(ERROR, __func__,
          "Did not write correct duration string into buffer, string was [%d] "
          "long with content [%s], should be [%d:%d:%d] (padded to width 2)",
          count, duration_str, hours, minutes, seconds);
    return NULL;
  }
  return duration_str;
}

char *get_current_duration_str(Timer *t) {
  int duration = get_duration(t);
  if (!duration) {
    char *no_duration = malloc(sizeof("00:00:00"));
    return no_duration;
  }
  return _duration_int_to_string(duration);
}

char *get_start_time_str(TimerResult *tr) {
  int string_size = sizeof("DAY, DD MON YYYY HH:MM:SS +ZONE");
  char *start_time_str = malloc(string_size * sizeof(char));
  if (!tr) {
    char *error = "No TimerResult";
    char *error_message = malloc(strlen(error) + 1);
    snprintf(error_message, strlen(error) + 1, "%s", error);
    return error_message;
  }
  if (!start_time_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return "Something went wrong :(";
  }
  struct tm *start_time_tm = localtime(&tr->start_time);
  strftime(start_time_str, string_size, "%a, %d %b %Y %H:%M:%S %z",
           start_time_tm);
  return start_time_str;
}

char *get_stop_time_str(TimerResult *tr) {
  int string_size = sizeof("DAY, DD MON YYYY HH:MM:SS +ZONE");
  char *end_time_str = malloc(string_size * sizeof(char));
  if (!tr) {
    char *error = "No TimerResult";
    char *error_message = malloc(strlen(error) + 1);
    snprintf(error_message, strlen(error) + 1, "%s", error);
    return error_message;
  }
  if (!end_time_str) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return "Something went wrong :(";
  }
  struct tm *end_time_tm = localtime(&tr->end_time);
  strftime(end_time_str, string_size, "%a, %d %b %Y %H:%M:%S %z", end_time_tm);
  return end_time_str;
}

char *get_final_duration_str(TimerResult *tr) {
  if (!tr) {
    char *error = "No TimerResult";
    char *error_message = malloc(strlen(error) + 1);
    snprintf(error_message, strlen(error) + 1, "%s", error);
    return error_message;
  }
  int duration = tr->duration;
  return _duration_int_to_string(duration);
}

StartInfo *from_request_body(struct mg_str *request_body) {
  if (!request_body) {
    t_log(ERROR, __func__, "No request_body");
    return NULL;
  }
  // init StartInfo struct
  StartInfo *si = malloc(sizeof(StartInfo));
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
    snprintf(name, 8, "not set");
  }

  char *var_key_client = "client";
  if (mg_http_get_var(request_body, var_key_client, client,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [client] could not be extracted from body. Not set or too "
          "long?");
    snprintf(client, 8, "not set");
  }

  char *var_key_project = "project";
  if (mg_http_get_var(request_body, var_key_project, project,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [project] could not be extracted from body. Not set or too "
          "long?");
    snprintf(project, 8, "not set");
  }

  char *var_key_description = "description";
  if (mg_http_get_var(request_body, var_key_description, description,
                      REQUEST_FIELD_MAX_SIZE) < 1) {
    t_log(ERROR, __func__,
          "Variable [description] could not be extracted from body. Not set or "
          "too long?");
    snprintf(description, 8, "not set");
  }

  return si;
log_error:
  t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
  return NULL;
};
