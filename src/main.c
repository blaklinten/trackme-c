#include <stdio.h>

#include "libgit2/include/git2.h"
#include "mongoose.h"

int main(int argc, char *argv[]) {
  git_libgit2_init();      // init libgit2
  struct mg_mgr mgr;       // Event manager
  mg_log_set(MG_LL_DEBUG); // Set log level
  mg_mgr_init(&mgr);       // Initialise event manager
  printf("Hello Git and Mongoose!\n");
  git_libgit2_shutdown();
  mg_mgr_init(&mgr);
  return EXIT_SUCCESS;
}
