#include "lidija.h"

LValue *l_value_new(enum LNodeType type, LClosure *closure) {
  LValue *value = malloc(sizeof(LValue));
  value->type = type;
  value->ref_count = 0;
  l_heap_add(closure->heap, value);
  return value;
}
