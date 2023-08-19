#include "db.h"
#include <libmongoc-1.0/mongoc.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cmocka.h>

void db_connect() {
  const char *uri_string = "mongodb://blaklinten:test-psw@localhost:27017";
  const char *TEST_DB_NAME = "trackme-test";
  mongoc_uri_t *uri;
  mongoc_client_t *client;
  mongoc_database_t *database;
  bson_t *command, reply;
  bson_error_t error;
  bool retval;

  /*
   * Required to initialize libmongoc's internals
   */
  mongoc_init();

  /*
   * Safely create a MongoDB URI object from the given string
   */
  uri = mongoc_uri_new_with_error(uri_string, &error);
  if (!uri) {
    fprintf(stderr,
            "failed to parse URI: %s\n"
            "error message:       %s\n",
            uri_string, error.message);
    fail();
  }

  /*
   * Create a new client instance
   */
  client = mongoc_client_new_from_uri(uri);
  if (!client) {
    fail();
  }

  /*
   * Register the application name so we can track it in the profile logs
   * on the server. This can also be done from the URI (see other examples).
   */
  mongoc_client_set_appname(client, "trackme-test-app");

  /*
   * Get a handle on the database "db_name"
   */
  database = mongoc_client_get_database(client, TEST_DB_NAME);

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

void db_save() { fail(); }

void db_get_by_name() { fail(); }

void db_get_by_client() { fail(); }

void db_get_by_project() { fail(); }
