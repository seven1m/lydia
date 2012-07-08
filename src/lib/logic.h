#ifndef LOGIC_H
#define LOGIC_H

bool l_to_bool(LValue *cond);
bool l_eq(LValue *v1, LValue *v2);
LValue *l_eval_if_expr(LValue *expr, LClosure *closure);

LValue *l_func_if(LValue *args, LClosure *closure);
LValue *l_func_while(LValue *args, LClosure *closure);
LValue *l_func_eq(LValue *args, LClosure *closure);
LValue *l_func_neq(LValue *args, LClosure *closure);
LValue *l_func_and(LValue *args, LClosure *closure);
LValue *l_func_or(LValue *args, LClosure *closure);

#endif
