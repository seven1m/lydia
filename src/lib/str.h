#ifndef STR_H
#define STR_H

LValue *l_func_str(LValue *args, LClosure *closure);
LValue *l_func_str_add(LValue *args, LClosure *closure);
LValue *l_func_str_eq(LValue *args, LClosure *closure);
char *l_str(LValue *value, LClosure *closure);

#endif
