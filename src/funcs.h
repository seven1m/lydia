#ifndef FUNCS_H
#define FUNCS_H

LValue *l_call_func(char *name, int argc, LNode **args, LValue *func, LClosure *closure);
LValue *l_eval_func_body(LValue *func, LValue *args, LClosure *closure);
void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure);
void l_create_funcs(LClosure *closure);
void l_create_globals(LClosure *closure);
void l_load_lib(LClosure *closure);

#endif
