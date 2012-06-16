#include "../lidija.h"

LValue *l_func_str(LValue *args, LClosure *closure) {
  LValue *value = l_value_new(L_STR_TYPE, closure);
  value->core.str = g_string_new("");
  char *s;
  int i;
  for(i=0; i<args->core.list->len; i++) {
    s = l_str(l_list_get(args, i));
    g_string_append(value->core.str, s);
    free(s);
  }
  return value;
}

LValue *l_func_str_add(LValue *args, LClosure *closure) {
  LValue *v1 = l_list_get(args, 0);
  LValue *v2 = l_list_get(args, 1);
  LValue *value = l_value_new(L_STR_TYPE, closure);
  value->core.str = g_string_new("");
  g_string_append(value->core.str, v1->core.str->str);
  g_string_append(value->core.str, v2->core.str->str);
  return value;
}

LValue *l_func_str_eq(LValue *args, LClosure *closure) {
  LValue *v1 = l_list_get(args, 0);
  LValue *v2 = l_list_get(args, 1);
  if(strcmp(v1->core.str->str, v2->core.str->str) == 0)
    return l_value_new(L_TRUE_TYPE, closure);
  else
    return l_value_new(L_FALSE_TYPE, closure);
}

// returns a c string representation for the given LValue
// (be sure to free the string when you're done)
char *l_str(LValue *value) {
  char *str;
  GString *str2;
  switch(value->type) {
    case L_NUM_TYPE:
      str = mpz_get_str(NULL, 10, value->core.num);
      break;
    case L_STR_TYPE:
      str = malloc(sizeof(char) * (value->core.str->len + 1));
      strcpy(str, value->core.str->str);
      break;
    case L_LIST_TYPE:
      str2 = g_string_new("[");
      char *s;
      int i, len = value->core.list->len;
      for(i=0; i<len; i++) {
        s = l_str(l_list_get(value, i));
        g_string_append(str2, s);
        free(s);
        if(i<len-1) g_string_append(str2, " ");
      }
      g_string_append(str2, "]");
      str = malloc(sizeof(char) * str2->len);
      strcpy(str, str2->str);
      g_string_free(str2, true);
      break;
    case L_TRUE_TYPE:
      str = malloc(sizeof(char) * 5);
      strcpy(str, "true");
      break;
    case L_FALSE_TYPE:
      str = malloc(sizeof(char) * 6);
      strcpy(str, "false");
      break;
    case L_NIL_TYPE:
      str = malloc(sizeof(char) * 4);
      strcpy(str, "nil");
      break;
    default:
      str = malloc(sizeof(char) * 1);
      strcpy(str, "");
  }
  return str;
}
