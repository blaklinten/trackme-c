#include "timer_result_list.h"
#include "log.h"
#include <assert.h>
#include <stdlib.h>

timer_result_list *create_empty_list() {
  timer_result_list *list = malloc(sizeof(timer_result_list));
  if (!list) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  list->value = NULL;
  list->next = NULL;
  list->previous = NULL;
  return list;
}

timer_result_list *create_list_from(TimerResult *first) {
  if (!first) {
    t_log(ERROR, __func__, "First element was NULL.");
    return NULL;
  }
  timer_result_list *list = create_empty_list();
  if (!list) {
    t_log(ERROR, __func__, "Failed to create empty list");
    return NULL;
  }

  list->value = first;
  list->previous = list;
  return list;
}

bool append_to_list(timer_result_list *list, TimerResult *element) {
  if (!list || !element) {
    t_log(ERROR, __func__, "List or element was NULL.");
    return false;
  }

  if (!list->value) {
    list->value = element;
    list->previous = list;
  } else {
    timer_result_list *append = create_list_from(element);
    list->previous->next = append;
    append->previous = list->previous;
    list->previous = append;
  }
  return true;
}

void free_list(timer_result_list *list) {
  if (!list) {
    t_log(INFO, __func__, "No list, will do nothing.");
    return;
  }

  timer_result_list *current = list;
  while (current) {
    timer_result_list *next = current->next; // Store the next node before freeing current
    if (current->value) {
      free_timer_result(current->value);
      current->value = NULL;
    }
    free(current); 
    current = next;
  }
}

int count_elements(timer_result_list *list) {
  if (!list) {
    t_log(ERROR, __func__, "List is NULL, we have no elements here");
    return 0;
  }
  int count = 0;
  if (!list->value) {
    return count;
  } else {
    count++;
  }
  timer_result_list *current = list->next;
  while (current) {
    count++;
    current = current->next;
  }
  return count;
}
