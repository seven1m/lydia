#include "../lidija.h"

LValue *l_list_get(LValue *list, long index) {
  if(index < list->core.list->len) {
    return g_array_index(list->core.list, LValue*, index);
  } else {
    return NULL;
  }
}

LValue *l_func_list_get(LValue *args, LClosure *closure) {
  LValue *list = l_list_get(args, 0);
  LValue *index = l_list_get(args, 1);
  return l_list_get(list, mpz_get_si(index->core.num));
}

LValue *l_func_map(LValue *args, LClosure *closure) {
  LValue *list = l_list_get(args, 0);
  LValue *func = l_list_get(args, 1);
  LValue *a;
  int i;
  for(i=0; i<list->core.list->len; i++) {
    a = l_value_new(L_LIST_TYPE, closure);
    a->core.list = g_array_sized_new(false, false, sizeof(LValue*), 1);
    g_array_insert_val(a->core.list, 0, g_array_index(list->core.list, LValue*, i));
    l_eval_func_body(func, a, closure);
  }
  return list;
}
