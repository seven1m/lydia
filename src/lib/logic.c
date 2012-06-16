#include "../lidija.h"

bool l_to_bool(LValue *cond) {
  return !(cond->type == L_NIL_TYPE ||
          (cond->type == L_FALSE_TYPE) ||
          (cond->type == L_NUM_TYPE && mpz_cmp_si(cond->core.num, 0) == 0) ||
          (cond->type == L_STR_TYPE && strcmp(cond->core.str->str, "") == 0) ||
          (cond->type == L_LIST_TYPE && cond->core.list->len == 0));
}

LValue *l_func_if(LValue *args, LClosure *closure) {
  LValue *value;
  LValue *cond = l_list_get(args, 0);
  if(args->core.list->len == 1) {
    // multi condition (else if)
    int i, len = args->core.list->len;
    LValue *expr;
    for(i=0; i<len; i+=2) {
      expr = g_array_index(args->core.list, LValue*, i);
      // TODO if i == len-1
      /*if(l_to_bool(v)) */
      return l_eval_if_expr(g_array_index(args->core.list, LValue*, i+1));
    }
  } else {
    // single condition
    LValue *true_expr = l_list_get(args, 1);
    LValue *false_expr = l_list_get(args, 2);
    if(true_expr == NULL) true_expr = l_value_new(L_NIL_TYPE, closure);
    if(false_expr == NULL) false_expr = l_value_new(L_NIL_TYPE, closure);
    if(l_to_bool(cond)) {
      return l_eval_if_expr(true_expr);
    } else {
      return l_eval_if_expr(false_expr);
    }
  }
  return value;
}

LValue *l_func_while(LValue *args, LClosure *closure) {
  LValue *ef = l_list_get(args, 0);
  LValue *wf = l_list_get(args, 1);
  l_assert_is(ef, L_FUNC_TYPE, L_ERR_MISSING_FUNC, closure);
  l_assert_is(wf, L_FUNC_TYPE, L_ERR_MISSING_FUNC, closure);
  LValue *value;
  while(l_to_bool(l_call_func("", 0, NULL, ef, closure))) {
    value = l_call_func("", 0, NULL, wf, closure);
  }
  return value;
}

LValue *l_func_eq(LValue *args, LClosure *closure) {
  LValue *v1 = l_list_get(args, 0);
  LValue *v2 = l_list_get(args, 1);
  if(v1->type == v2->type && (v1->type == L_TRUE_TYPE || v1->type == L_FALSE_TYPE)) {
    return l_value_new(L_TRUE_TYPE, closure);
  // TODO
  //} else if(v1->type == L_LIST_TYPE && v2->type == L_LIST_TYPE) {
  //  return l_func_list_eq(args, closure);
  } else if(v1->type == L_NUM_TYPE && v2->type == L_NUM_TYPE) {
    return l_func_num_eq(args, closure);
  } else if(v1->type == L_STR_TYPE && v2->type == L_STR_TYPE) {
    return l_func_str_eq(args, closure);
  } else {
    return l_value_new(L_FALSE_TYPE, closure);
  }
}

LValue *l_func_neq(LValue *args, LClosure *closure) {
  if(l_func_eq(args, closure)->type == L_TRUE_TYPE)
    return l_value_new(L_FALSE_TYPE, closure);
  else
    return l_value_new(L_TRUE_TYPE, closure);
}

LValue *l_func_and(LValue *args, LClosure *closure) {
  LValue *v1 = l_list_get(args, 0);
  LValue *v2 = l_list_get(args, 1);
  if(l_to_bool(v1) && l_to_bool(v2))
    return l_value_new(L_TRUE_TYPE, closure);
  else
    return l_value_new(L_FALSE_TYPE, closure);
}

LValue *l_func_or(LValue *args, LClosure *closure) {
  LValue *v1 = l_list_get(args, 0);
  LValue *v2 = l_list_get(args, 1);
  if(l_to_bool(v1) || l_to_bool(v2))
    return l_value_new(L_TRUE_TYPE, closure);
  else
    return l_value_new(L_FALSE_TYPE, closure);
}
