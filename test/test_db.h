#ifndef TEST_DB_H
#define TEST_DB_H

#define TEST_DB_FILE "./TEST_TRACKME_DB"

void test_db_save(void **);
void test_db_save_NULL(void **);
void test_db_get_by(void **);
void test_db_insert_and_get(void **);

#endif // !TEST_DB_H
