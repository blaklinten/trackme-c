#include "../src/util/timer_result_list.h"
#include "../src/util/log.h"
#include "unit_test.h"
#include <cmocka.h>
#include <stdlib.h>

/*** Helper functions ***/

TimerResult * _copy_timer_result(TimerResult * orig_tr){
  if (!orig_tr) {
    t_log(ERROR, __func__, "No TimerResult to copy!");
    return NULL;
  }
  // init TimerResult struct
  TimerResult *copy = calloc(1, sizeof(TimerResult));
  if (!copy) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  StartInfo *si = copy_start_info(orig_tr->info);
  if (!si){
    t_log(ERROR, __func__, "Could not copy start info.");
    return NULL;
  }
  copy->info = si;
  copy->start_time = orig_tr->start_time;
  copy->end_time = orig_tr->end_time;
  copy->duration = orig_tr->duration;
  return copy;
}

void test_list_empty_create(void **state) {
  // Given
  // When
  timer_result_list *empty_list = create_empty_list();

  // Then
  assert_non_null(empty_list);
  assert_int_equal(0, count_elements(empty_list));

  // Finally
  free_list(empty_list);
}

void test_list_create_from_NULL(void **state) {
  // Given
  // When
  timer_result_list *null_list = create_list_from(NULL);

  // Then
  assert_null(null_list);

  // Finally
  free_list(null_list);
}

void test_list_create_from_timer_result(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  TimerResult *result_1 = _copy_timer_result(s->TEST_TIMER_RESULT);

  // When
  timer_result_list *single_element_list = create_list_from(result_1);

  // Then
  assert_non_null(single_element_list);
  assert_int_equal(1, count_elements(single_element_list));

  // Finally
  free_list(single_element_list);
}

void test_list_add_element(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  timer_result_list *initially_non_empty_list = create_list_from(_copy_timer_result(s->TEST_TIMER_RESULT));
  timer_result_list *initially_empty_list = create_empty_list();

  // When
  bool append_empty = append_to_list(initially_empty_list, _copy_timer_result(s->TEST_TIMER_RESULT));
  bool append_non_empty = append_to_list(initially_non_empty_list, _copy_timer_result(s->TEST_TIMER_RESULT));
  // Then

  // Add new element
  assert_true(append_empty);
  assert_int_equal(1, count_elements(initially_empty_list));
  assert_true(append_non_empty);
  assert_int_equal(2, count_elements(initially_non_empty_list));

  // Finally
  free_list(initially_non_empty_list);
  free_list(initially_empty_list);
}

void test_list_add_invalid_element(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  timer_result_list *initially_non_empty_list = create_list_from(_copy_timer_result(s->TEST_TIMER_RESULT));
  timer_result_list *initially_empty_list = create_empty_list();

  // When
  // Then

  // Add illegal element
  assert_false(append_to_list(initially_empty_list, NULL));
  assert_int_equal(0, count_elements(initially_empty_list));
  assert_false(append_to_list(initially_non_empty_list, NULL));
  assert_int_equal(1, count_elements(initially_non_empty_list));

  // Finally
  free_list(initially_non_empty_list);
  free_list(initially_empty_list);
}

void test_list_count_element(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  timer_result_list *empty_list = create_empty_list();

  // When
  for (int i = 0; i < 10; i++) {
    append_to_list(empty_list, _copy_timer_result(s->TEST_TIMER_RESULT));
  }

  // Then
  assert_int_equal(10, count_elements(empty_list));

  // Finally
  free_list(empty_list);
}

void test_list_free(void **state) {
  // Given
  test_state_t *s = (test_state_t *)*state;
  timer_result_list *list = create_list_from(_copy_timer_result(s->TEST_TIMER_RESULT));
  append_to_list(list, _copy_timer_result(s->TEST_TIMER_RESULT));

  // When
  free_list(list);

  // Then
  /* No memory leaks suggest successful free */

  // Finally
}
