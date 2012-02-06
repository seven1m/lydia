#ifndef VALUE_H
#define VALUE_H

#include <glib.h>

typedef struct LValue {
  LNode *node;
  int ref_count;
} LValue;

LValue *l_value_new(LNode *node);

#endif
