#include <libmongoc-1.0/mongoc.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

void db_connect() {
  bson_t *command_ptr = NULL, reply;
  bson_error_t error;
  bool sucess = false;

  /*
   * Do work. This example pings the database.
   */
  command_ptr = BCON_NEW("ping", BCON_INT32(1));

  sucess = mongoc_client_command_simple(db_client_ptr, "admin", command_ptr,
                                        NULL, &reply, &error);

  if (!sucess) {
    fprintf(stderr, "Failed to ping db\n");
    fail();
  }

  /*
   * Release our handles and clean up libmongoc
   */
  bson_destroy(&reply);
  bson_destroy(command_ptr);
}


  /*
   * Do work. This example pings the database.
   */
  command = BCON_NEW("ping", BCON_INT32(1));

  retval = mongoc_client_command_simple(client, "admin", command, NULL, &reply,
                                        &error);

  if (!retval) {
    fprintf(stderr, "error message: %s\n", error.message);
    fail();
  }

  /*
   * Release our handles and clean up libmongoc
   */
  bson_destroy(command);
  mongoc_database_destroy(database);
  mongoc_uri_destroy(uri);
  mongoc_client_destroy(client);
  mongoc_cleanup();
}
