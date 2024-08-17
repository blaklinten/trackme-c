#include "unit_test.h"
#include "../src/util/bson_list.h"
#include <bson/bson.h>

void test_list_create(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;
  bson_t *td1 = s->test_document_1;

  // When
  bson_t_list *null_list = create_list_from(NULL);

  bson_t_list *empty_list = create_empty_list();

  bson_t_list *single_element_list = create_list_from(td1);

  // Then
  assert_null(null_list);

  assert_non_null(empty_list);
  assert_int_equal(0, count_elements(empty_list));

  assert_non_null(single_element_list);
  assert_int_equal(1, count_elements(single_element_list));

   // Finally
  free_list(null_list);
  free_list(empty_list);
  free_list(single_element_list);
};

void test_list_add_element(void **state){
  // Given
  test_state_t *s = (test_state_t *)*state;
  bson_t *td1 = s->test_document_1;
  bson_t_list *non_empty_list = create_list_from(td1);
  bson_t_list *empty_list = create_empty_list();

  // When
  for (int i = 1; i < 10; i++){
    assert_true(append_to_list(non_empty_list, bson_new()));
  }
  assert_false(append_to_list(non_empty_list, NULL));

  assert_false(append_to_list(empty_list, NULL));
  assert_true(append_to_list(empty_list, bson_new()));

  // Then
  assert_non_null(empty_list);
  assert_int_equal(1, count_elements(empty_list));

  assert_non_null(non_empty_list);
  assert_int_equal(10, count_elements(non_empty_list));

   // Finally
  free_list(non_empty_list);
  free_list(empty_list);
};
  
void test_list_count_element(void **state){
  // Given
  bson_t_list *non_empty_list = create_empty_list();
  // When
  for (int i = 0; i < 10; i++){
    append_to_list(non_empty_list, bson_new());
  }

  // Then
  assert_int_equal(10, count_elements(non_empty_list));

  free_list(non_empty_list);
};
