#include "../lidija.h"

LValue *l_func_num_mult(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_mul(value->core.num, l_list_get(args, 0)->core.num, l_list_get(args, 1)->core.num);
  return value;
}
