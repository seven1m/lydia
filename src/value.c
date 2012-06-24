#include "lidija.h"

LValue *l_value_new(enum LNodeType type, LClosure *closure) {
  LValue *value = GC_MALLOC(sizeof(LValue));
  value->type = type;
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

void l_value_free(LValue *value) {
  if(value->type == L_FUNC_TYPE) {
    l_closure_free(value->core.func.closure);
    // TODO free more stuff
  }
  // TODO free more stuff
  value = NULL;
  GC_free(value);
}
