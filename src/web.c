#include "web.h"
#include "../lib/ctemplate.h"
#include "util/log.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void create_index_html(char *name, char *client, char *project,
                       char *description, char *current_duration) {
  TMPL_varlist *tmpl_vars = NULL;
  tmpl_vars = TMPL_add_var(NULL, "name", name, "client", client, "project",
                           project, "description", description, "duration",
                           current_duration, NULL);

  FILE *index_html_file = fopen("index.html", "w+");
  int err = TMPL_write("templates/index.tmpl", NULL, NULL, tmpl_vars,
                       index_html_file, stderr);
  TMPL_free_varlist(tmpl_vars);
  fclose(index_html_file);

  if (err) {
    t_log(ERROR, __func__, "Failed at writing HTML file");
    // return static error page?
  }
}

void create_start_timer_html() {
  FILE *start_html = fopen("start_timer.html", "w+");
  int err = TMPL_write("templates/start_timer.tmpl", NULL, NULL, NULL,
                       start_html, stderr);
  fclose(start_html);

  if (err) {
    t_log(ERROR, __func__, "Failed at writing HTML file");
    // return static error page?
  }
}

void create_stop_timer_html(char *name, char *client, char *project,
                            char *description, char *start_time, char *end_time,
                            char *final_duration) {
  TMPL_varlist *tmpl_vars = NULL;
  tmpl_vars =
      TMPL_add_var(NULL, "name", name, "client", client, "project", project,
                   "description", description, "start_time", start_time,
                   "end_time", end_time, "duration", final_duration, NULL);

  FILE *stop_html = fopen("stop_timer.html", "w+");
  int err = TMPL_write("templates/stop_timer.tmpl", NULL, NULL, tmpl_vars,
                       stop_html, stderr);
  fclose(stop_html);

  if (err) {
    t_log(ERROR, __func__, "Failed at writing HTML file");
    // return static error page?
  }
}
