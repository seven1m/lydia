#include "../lidija.h"

LValue *l_func_num_add(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init(value->core.num);
  mpz_add(value->core.num, l_list_get(args, 0)->core.num, l_list_get(args, 1)->core.num);
  return value;
}

LValue *l_func_num_sub(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init(value->core.num);
  mpz_sub(value->core.num, l_list_get(args, 0)->core.num, l_list_get(args, 1)->core.num);
  return value;
}

LValue *l_func_num_mul(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init(value->core.num);
  mpz_mul(value->core.num, l_list_get(args, 0)->core.num, l_list_get(args, 1)->core.num);
  return value;
}

LValue *l_func_num_div(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init(value->core.num);
  mpz_tdiv_q(value->core.num, l_list_get(args, 0)->core.num, l_list_get(args, 1)->core.num);
  return value;
}

int l_num_cmp(LValue *args) {
  return mpz_cmp(l_list_get(args, 0)->core.num, l_list_get(args, 1)->core.num);
}

LValue *l_func_num_eq(LValue *args, LClosure *closure) {
  return l_value_new(l_num_cmp(args) == 0 ? L_TRUE_TYPE : L_FALSE_TYPE, closure);
}

LValue *l_func_num_neq(LValue *args, LClosure *closure) {
  return l_value_new(l_num_cmp(args) != 0 ? L_TRUE_TYPE : L_FALSE_TYPE, closure);
}

LValue *l_func_num_gt(LValue *args, LClosure *closure) {
  return l_value_new(l_num_cmp(args) > 0 ? L_TRUE_TYPE : L_FALSE_TYPE, closure);
}

LValue *l_func_num_gte(LValue *args, LClosure *closure) {
  return l_value_new(l_num_cmp(args) >= 0 ? L_TRUE_TYPE : L_FALSE_TYPE, closure);
}

LValue *l_func_num_lt(LValue *args, LClosure *closure) {
  return l_value_new(l_num_cmp(args) < 0 ? L_TRUE_TYPE : L_FALSE_TYPE, closure);
}

LValue *l_func_num_lte(LValue *args, LClosure *closure) {
  return l_value_new(l_num_cmp(args) <= 0 ? L_TRUE_TYPE : L_FALSE_TYPE, closure);
}
