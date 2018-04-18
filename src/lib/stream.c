#include "../lydia.h"

// TODO: LOTS of work to do here; we'll fake it for now

LValue *l_func_out(LValue *args, LClosure *closure) {
  LValue *value = l_func_str(args, closure);
  printf("%s\n", value->core.str->str);
  return value;
}
