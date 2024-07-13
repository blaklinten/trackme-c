#include <stdio.h>

#include "log.h"
#include "mongoose.h"

#include "db.h"
#include "timer.h"

int main() {
  struct mg_mgr mgr;       // Event manager
  mg_log_set(MG_LL_DEBUG); // Set log level
  mg_mgr_init(&mgr);       // Initialise event manager
  char *db_uri = "/this/is/db/URI";
  if (!init_db(db_uri)) {
    t_log(ERROR, __func__, "failed to initialize db. \n");
    return EXIT_FAILURE;
  }
  printf("Hello Mongoose!\n");

  Timer t;
  reset(&t);

  mg_mgr_free(&mgr);
  return EXIT_SUCCESS;
}
