#ifndef TEST_DB_H
#define TEST_DB_H

#include <bson/bson.h>
#include <mongoc/mongoc.h>

void test_db_connect();
void test_db_save(void **);
void test_db_get_by(void **);
void test_db_insert_and_get(void **);

#endif // !TEST_DB_H
