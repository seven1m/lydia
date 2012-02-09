#include "../lidija.h"

LValue *l_list_get(LValue *list, long index) {
  return g_array_index(list->core.list, LValue*, index);
}

LValue *l_func_list_get(LValue *args, LClosure *closure) {
  LValue *list = l_list_get(args, 0);
  LValue *index = l_list_get(args, 1);
  return l_list_get(list, mpz_get_si(index->core.num));
}
