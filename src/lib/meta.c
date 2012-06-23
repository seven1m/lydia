#include "../lidija.h"

LValue *l_func_heap_to_list(LValue *args, LClosure *closure) {
  LValue *val, *list = l_value_new(L_LIST_TYPE, closure);
  list->core.list = g_array_sized_new(false, false, sizeof(LValue*), closure->heap->len);
  int i;
  for(i=0; i<closure->heap->len; i++) {
    val = g_ptr_array_index(closure->heap, i);
    g_array_insert_val(list->core.list, i, val);
  }
  return list;
}
