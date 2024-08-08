#include <bson/bson.h>
#include <stdio.h>
#include <unistd.h>

#include "db.h"
#include "log.h"
#include "mongoose.h"
#include "timer.h"
#include "track_me.h"
#include "web.h"

Timer t;

static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (mg_match(hm->uri, mg_str("/"), NULL)) {
      char *current_duration = get_current_duration_str(&t);
      create_index_html(t.name, t.client, t.project, t.description,
                        current_duration);
    } else if (mg_match(hm->uri, mg_str("/start_timer.html"), NULL)) {
      create_start_timer_html();
    } else if (mg_match(hm->uri, mg_str("/stop_timer.html"), NULL)) {
      t_log(INFO, __func__, "Stopping timer...");
      TimerResult *tr = stop(&t);
      if (!tr) {
        t_log(INFO, __func__, "Could not stop timer, returning NULL...");
        create_stop_timer_html(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
      } else {
        char *start_time_str = get_start_time_str(tr);
        char *end_time_str = get_stop_time_str(tr);
        char *final_duration = get_final_duration_str(tr);
        create_stop_timer_html(tr->name, tr->client, tr->project,
                               tr->description, start_time_str, end_time_str,
                               final_duration);
      }
    }
    t_log(INFO, __func__, "Serving local dir");
    struct mg_http_serve_opts opts = {.root_dir = "./"}; // Serve local dir
    mg_http_serve_dir(c, ev_data, &opts);
  }
}

int main() {
  struct mg_mgr mgr;       // Event manager
  mg_log_set(MG_LL_DEBUG); // Set log level
  mg_mgr_init(&mgr);       // Initialise event manager
  // // char *db_uri = "/this/is/db/URI";
  // // int err = init_db(db_uri);
  // // if (err) {
  // //   t_log(ERROR, __func__, "failed to initialize db. \n");
  // //   return EXIT_FAILURE;
  // // }
  printf("Hello Mongoose!\n");

  reset(&t);
  StartInfo s = {"T_Name", "T_Client", "T_Project", "A little test"};
  start(&t, &s);
  // Setup listener
  mg_http_listen(&mgr, "http://localhost:8000", ev_handler, NULL);

  // Event loop
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }

  // Cleanup
  mg_mgr_free(&mgr);
  return EXIT_SUCCESS;
}
