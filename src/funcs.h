#ifndef FUNCS_H
#define FUNCS_H

void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure);
void l_create_funcs(LClosure *closure);

#endif
