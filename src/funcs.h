#ifndef FUNCS_H
#define FUNCS_H

LValue *l_eval_call_node(LNode *node, LValue *func, LClosure *closure);
LValue *l_eval_call_args(LNode *node, LValue *func, LClosure *closure, LClosure *cl);
void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure);
void l_create_funcs(LClosure *closure);
void l_create_globals(LClosure *closure);
void l_load_lib(LClosure *closure);

#endif
