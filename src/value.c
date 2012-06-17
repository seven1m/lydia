#include "lidija.h"

LValue *l_value_new(enum LNodeType type, LClosure *closure) {
  LValue *value = malloc(sizeof(LValue));
  value->type = type;
  value->flags = 0;
  value->ref_count = 0;
  l_heap_add(closure->heap, value);
  return value;
}

LValue *l_value_new_builtin(enum LNodeType type, LClosure *closure) {
  LValue *value = l_value_new(type, closure);
  value->flags |= L_VALUE_BUILTIN_FLAG;
  return value;
}

LValue *l_func_new(struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure) {
  LValue *func = l_value_new(L_FUNC_TYPE, closure);
  func->flags |= L_VALUE_BUILTIN_FLAG;
  func->core.func.ptr = ptr;
  func->core.func.closure = closure;
  func->core.func.argc = 0;
  func->core.func.exprc = 0;
  return func;
}

void l_value_free(LValue *value) {
  if(value->type == L_FUNC_TYPE) {
    l_closure_free(value->core.func.closure);
    // TODO free more stuff
  }
  // TODO free more stuff
  free(value);
}

bool l_value_builtin(LValue *value) {
  return (value->flags & L_VALUE_BUILTIN_FLAG) != 0;
}
