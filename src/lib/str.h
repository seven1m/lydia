#ifndef STR_H
#define STR_H

bool l_str_eq(LValue *s1, LValue *s2);
char *l_str(LValue *value);

LValue *l_func_str(LValue *args, LClosure *closure);
LValue *l_func_str_add(LValue *args, LClosure *closure);

stringbuf *make_stringbuf(char *str);
void concat_stringbuf(stringbuf *buf, char *str);

#endif
