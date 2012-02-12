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
  l_assert_is(list, L_LIST_TYPE, L_ERR_MISSING_LIST, closure);
  l_assert_is(index, L_NUM_TYPE, L_ERR_MISSING_INDEX, closure);
  return l_list_get(list, mpz_get_si(index->core.num));
}

LValue *l_func_count(LValue *args, LClosure *closure) {
  LValue *list = l_list_get(args, 0);
  l_assert_is(list, L_LIST_TYPE, L_ERR_MISSING_LIST, closure);
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init_set_ui(value->core.num, list->core.list->len);
  return value;
}

LValue *l_func_first(LValue *args, LClosure *closure) {
  LValue *list = l_list_get(args, 0);
  l_assert_is(list, L_LIST_TYPE, L_ERR_MISSING_LIST, closure);
  LValue *value = l_list_get(list, 0);
  if(value == NULL) value = l_value_new(L_NIL_TYPE, closure);
  return value;
}

LValue *l_func_rest(LValue *args, LClosure *closure) {
  LValue *list = l_list_get(args, 0);
  LValue *value = l_value_new(L_LIST_TYPE, closure);
  value->core.list = g_array_sized_new(false, false, sizeof(LValue*), list->core.list->len-1);
  int i;
  for(i=1; i<list->core.list->len; i++) {
    g_array_insert_val(value->core.list, i-1, g_array_index(list->core.list, LValue*, i));
  }
  return value;
}

LValue *l_func_list_add(LValue *args, LClosure *closure) {
  LValue *l1 = l_list_get(args, 0);
  LValue *l2 = l_list_get(args, 1);
  LValue *value = l_value_new(L_LIST_TYPE, closure);
  value->core.list = g_array_sized_new(false, false, sizeof(LValue*), l1->core.list->len + l2->core.list->len);
  g_array_append_vals(value->core.list, l1->core.list->data, l1->core.list->len);
  g_array_append_vals(value->core.list, l2->core.list->data, l2->core.list->len);
  return value;
}
