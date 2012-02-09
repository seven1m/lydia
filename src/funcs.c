#include "lidija.h"

// inserts given function as given name in given closure
void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure) {
  g_hash_table_insert(closure->vars, name, l_func_new(ptr, closure));
}

// creates all built-in functions in the given closure
void l_create_funcs(LClosure *closure) {
  l_insert_func("*", l_func_num_mult, closure);
  l_insert_func("->", l_func_list_get, closure);
  l_insert_func("str", l_func_str, closure);
  l_insert_func("out", l_func_out, closure);
}
