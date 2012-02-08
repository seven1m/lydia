#include "lidija.h"

LValue *num_mult(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init(value->core.num);
  LValue *a = g_array_index(args->core.list, LValue*, 0);
  LValue *b = g_array_index(args->core.list, LValue*, 1);
  mpz_mul(value->core.num, a->core.num, b->core.num);
  return value;
}
