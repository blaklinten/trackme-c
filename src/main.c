#include <stdio.h>

#include "mongoose.h"

#include "timer.h"

int main(int argc, char *argv[]) {
  struct mg_mgr mgr;       // Event manager
  mg_log_set(MG_LL_DEBUG); // Set log level
  mg_mgr_init(&mgr);       // Initialise event manager
  printf("Hello Git and Mongoose!\n");

  Timer t;
  reset(&t);

  mg_mgr_free(&mgr);
  return EXIT_SUCCESS;
}
