#ifndef BSON_LIST_H
#define BSON_LIST_H
#define BSON_LIST_H

#include "../timer.h"

typedef struct timer_result_list {
  TimerResult *value;
  struct timer_result_list *next;
  struct timer_result_list *previous;
} timer_result_list;

timer_result_list *create_empty_list();
timer_result_list *create_list_from(TimerResult *element);
bool append_to_list(timer_result_list *list, TimerResult *element);
void free_list(timer_result_list *);
int count_elements(timer_result_list *);

#endif // !BSON_LIST_H
