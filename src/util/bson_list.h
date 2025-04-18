#ifndef BSON_LIST_H
#define BSON_LIST_H
#define BSON_LIST_H

#include <bson/bson.h>

typedef struct bson_t_list {
  bson_t *value;
  struct bson_t_list *next;
  struct bson_t_list *previous;
} bson_t_list;

bson_t_list *create_empty_list();
bson_t_list *create_list_from(bson_t *element);
bool append_to_list(bson_t_list *list, bson_t *element);
void free_list(bson_t_list *);
int count_elements(bson_t_list *);

#endif // !BSON_LIST_H
