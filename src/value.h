#ifndef VALUE_H
#define VALUE_H

#include <glib.h>

typedef struct airb_value {
  airb_node* node;
  int ref_count;
} airb_value;

airb_value* airb_value_new(airb_node*);

#endif
