#ifndef LIST_H
#define LIST_H

bool l_list_eq(LValue *l1, LValue *l2);

LValue *l_list_get(LValue *list, long index);
LValue *l_func_list_get(LValue *args, LClosure *closure);
LValue *l_func_count(LValue *args, LClosure *closure);
LValue *l_func_first(LValue *args, LClosure *closure);
LValue *l_func_rest(LValue *args, LClosure *closure);
LValue *l_func_list_add(LValue *args, LClosure *closure);

#endif
