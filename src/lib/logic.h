#ifndef LOGIC_H
#define LOGIC_H

bool l_to_bool(LValue *cond);

LValue *l_func_if(LValue *args, LClosure *closure);
LValue *l_func_eq(LValue *args, LClosure *closure);
LValue *l_func_neq(LValue *args, LClosure *closure);
LValue *l_func_and(LValue *args, LClosure *closure);
LValue *l_func_or(LValue *args, LClosure *closure);

#endif
