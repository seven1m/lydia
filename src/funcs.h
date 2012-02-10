#ifndef FUNCS_H
#define FUNCS_H

void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure);
void l_create_funcs(LClosure *closure);
void l_create_globals(LClosure *closure);
void l_load_lib(LClosure *closure);
bool l_to_bool(LValue *cond);

LValue *l_func_if(LValue *args, LClosure *closure);
LValue *l_func_add(LValue *args, LClosure *closure);
LValue *l_func_eq(LValue *args, LClosure *closure);
LValue *l_func_neq(LValue *args, LClosure *closure);
LValue *l_func_and(LValue *args, LClosure *closure);
LValue *l_func_or(LValue *args, LClosure *closure);
LValue *l_func_type(LValue *args, LClosure *closure);
LValue *l_func_require(LValue *args, LClosure *closure);
LValue *l_call_func(char *name, int argc, LNode **args, LValue *func, LClosure *closure);
LValue *l_eval_func_body(LValue *func, LValue *args, LClosure *closure);

#endif
