#include <glib.h>
#include "lidija.h"

LValue *l_value_new(LNode *node) {
  LValue *value = malloc(sizeof(LValue));
  value->node = node;
  value->ref_count = 0;
  return value;
}
