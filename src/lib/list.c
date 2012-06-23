#include "../lidija.h"

LValue *l_list_get(LValue *list, long index) {
  if(index < list->core.list->length) {
    return *((LValue**)vector_get(list->core.list, index));
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
  mpz_init_set_ui(value->core.num, list->core.list->length);
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
  if(list->core.list->length > 1) {
    value->core.list = subvector(list->core.list, 1, list->core.list->length);
  } else {
    value->core.list = create_vector();
  }
  return value;
}

LValue *l_func_list_add(LValue *args, LClosure *closure) {
  LValue *l1 = l_list_get(args, 0);
  LValue *l2 = l_list_get(args, 1);
  LValue *value = l_value_new(L_LIST_TYPE, closure);
  value->core.list = subvector(l1->core.list, 0, l1->core.list->length);
  int i;
  for(i=0; i<l2->core.list->length; i++) {
    vector_add(value->core.list, l2->core.list->data[i], l2->core.list->sizes[i]);
  }
  return value;
}

bool l_list_eq(LValue *l1, LValue *l2) {
  if(l1->core.list->length == 0 && l2->core.list->length == 0) {
    return true;
  } else if(l1->core.list->length == l2->core.list->length) {
    int i;
    for(i=0; i<l1->core.list->length; i++) {
      if(!l_eq(l_list_get(l1, i), l_list_get(l2, i))) {
        return false;
      }
    }
    return true;
  }
  return false;
}
