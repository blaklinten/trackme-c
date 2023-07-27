#include "timer.h"

time_t start_timer(struct Timer *t, struct StartInfo *si) {
  t->name = si->name;
  t->client = si->client;
  t->project = si->project;
  t->description = si->description;
  t->start_time = time(NULL);
  t->end_time = -1;

  return t->start_time;
}

struct Timer *create_timer()
{
  struct Timer *t_ptr = malloc(sizeof(struct Timer));
  
  t_ptr->name = NULL;
  t_ptr->client = NULL;
  t_ptr->project = NULL;
  t_ptr->description = NULL;
  t_ptr->start_time = -1;
  t_ptr->end_time = -1;

  return t_ptr;
}
