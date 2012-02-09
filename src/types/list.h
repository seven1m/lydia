#ifndef LIST_H
#define LIST_H

LValue *l_list_get(LValue *list, long index);
LValue *l_func_list_get(LValue *args, LClosure *closure);

#endif
