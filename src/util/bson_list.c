#include "bson_list.h"
#include "log.h"
#include <assert.h>

bson_t_list *create_empty_list() {
  bson_t_list *list = malloc(sizeof(bson_t_list));
  if (!list) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }
  list->value = NULL;
  list->next = NULL;
  list->previous = NULL;
  return list;
}

bson_t_list *create_list_from(bson_t *first) {
  if (!first){
    t_log(ERROR, __func__, "First element was NULL.");
    return NULL;
  }
  bson_t_list *list = create_empty_list();
  if (!list) {
    t_log(ERROR, __func__, "Malloc: could not allocate enough memory.");
    return NULL;
  }

  list->value = first;
  list->previous = list;
  return list;
}

bool append_to_list(bson_t_list *list, bson_t *element) {
  if (!list || !element){
    t_log(ERROR, __func__, "List or element was NULL.");
    return false;
  }

  if (!list->value) {
    list->value = element;
    list->previous = list;
  } else {
    bson_t_list *append = create_list_from(element);
    list->previous->next = append;
    append->previous = list->previous;
    list->previous = append;
  }
  return true;
}

void free_list(bson_t_list *list) {
  if (!list) {
    t_log(INFO, __func__, "List is empty, will do nothing.");
    return;
  }

  bson_t_list *current = list;
  while (current->next) {
    if (current->value) {
      bson_destroy(current->value);
      current->value = NULL;
    }
    current = current->next;
    free(current->previous);
    current->previous = NULL;
  }
  bson_destroy(current->value);
  current->value = NULL;

  free(current);
}

int count_elements(bson_t_list *list) {
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
  bson_t_list *current = list->next;
  while (current) {
    count++;
    current = current->next;
  }
  return count;
}
