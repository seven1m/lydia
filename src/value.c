#include "lidija.h"

// note: have to use void* because of circular ref
LValue *l_value_new(enum LNodeType type, void *closure) {
  LValue *value = malloc(sizeof(LValue));
  value->type = type;
  value->ref_count = 0;
  l_heap_add(((LClosure*)closure)->heap, value);
  return value;
}
