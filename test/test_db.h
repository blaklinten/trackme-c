#ifndef TEST_DB_H
#define TEST_DB_H

void test_db_connect();
void test_db_save(void **);
void test_db_get_by(void **);
void test_db_insert_and_get(void **);
void test_db_create_list(void **);
void test_db_free_list(void **);
void test_db_count_list_element(void **);

#endif // !TEST_DB_H
