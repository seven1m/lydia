#include "lidija.h"

LValue *l_value_new(enum LNodeType type, LClosure *closure) {
  LValue *value = malloc(sizeof(LValue));
  value->type = type;
  value->ref_count = 0;
  l_heap_add(closure->heap, value);
  return value;
}

LValue *l_func_new(struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure) {
  LValue *func = l_value_new(L_FUNC_TYPE, closure);
  func->core.func.ptr = ptr;
  func->core.func.closure = closure;
  func->core.func.argc = 0;
  func->core.func.exprc = 0;
  return func;
}

