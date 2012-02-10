#include "lidija.h"

bool l_to_bool(LValue *cond) {
  return (cond->type == L_NIL_TYPE ||
         (cond->type == L_FALSE_TYPE) ||
         (cond->type == L_NUM_TYPE && mpz_cmp_si(cond->core.num, 0) == 0) ||
         (cond->type == L_STR_TYPE && strcmp(cond->core.str->str, "") == 0) ||
         (cond->type == L_LIST_TYPE && cond->core.list->len == 0));
}

LValue *l_func_if(LValue *args, LClosure *closure) {
  LValue *value;
  LValue *cond = l_list_get(args, 0);
  LValue *true_expr = l_list_get(args, 1);
  LValue *false_expr = l_list_get(args, 2);
  if(true_expr == NULL) true_expr = l_value_new(L_NIL_TYPE, closure);
  if(false_expr == NULL) false_expr = l_value_new(L_NIL_TYPE, closure);
  if(l_to_bool(cond)) {
    value = (false_expr->type == L_FUNC_TYPE) ? l_call_func("", 0, NULL, false_expr, closure) : false_expr;
  } else {
    value = (true_expr->type == L_FUNC_TYPE) ? l_call_func("", 0, NULL, true_expr, closure) : true_expr;
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
