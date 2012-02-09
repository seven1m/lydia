#include "../lidija.h"

LValue *l_func_str(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_STR_TYPE, closure);
  value->core.str = g_string_new("");
  char *s;
  int i;
  for(i=0; i<args->core.list->len; i++) {
    s = l_str(l_list_get(args, i), closure);
    g_string_append(value->core.str, s);
    free(s);
  }
  return value;
}

// returns a c string representation for the given LValue
// (be sure to free the string when you're done)
char *l_str(LValue *value, LClosure *closure) {
  char *str;
  LValue *str2;
  switch(value->type) {
    case L_NUM_TYPE:
      str = mpz_get_str(NULL, 10, value->core.num);
      break;
    case L_STR_TYPE:
      str = malloc(sizeof(char) * (value->core.str->len + 1));
      strcpy(str, value->core.str->str);
      break;
    case L_LIST_TYPE:
      str2 = l_func_str(value, closure);
      str = malloc(sizeof(char) * (str2->core.str->len + 1));
      strcpy(str, str2->core.str->str);
      break;
    default:
      str = malloc(sizeof(char) * 1);
      strcpy(str, "");
  }
  return str;
}
