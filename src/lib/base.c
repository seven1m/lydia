#include "../lidija.h"

LValue *l_func_add(LValue *args, LClosure *closure) {
  LValue *v1 = l_list_get(args, 0);
  LValue *v2 = l_list_get(args, 1);
  if(v1->type == L_LIST_TYPE && v2->type == L_LIST_TYPE) {
    return l_func_list_add(args, closure);
  } else if(v1->type == L_NUM_TYPE && v2->type == L_NUM_TYPE) {
    return l_func_num_add(args, closure);
  } else if(v1->type == L_STR_TYPE && v2->type == L_STR_TYPE) {
    return l_func_str_add(args, closure);
  } else {
    return l_value_new(L_NIL_TYPE, closure);
  }
}

LValue *l_func_type(LValue *args, LClosure *closure) {
  LValue *value = l_list_get(args, 0);
  char *types[9] = {
    "nil",
    "boolean",
    "boolean",
    "error",
    "number",
    "string",
    "variable",
    "list",
    "function"
  };
  LValue *repr = l_value_new(L_STR_TYPE, closure);
  repr->core.str = g_string_new(types[value->type]);
  return repr;
}

GHashTable *l_loaded_libs;

LValue *l_func_require(LValue *args, LClosure *closure) {
  if(l_loaded_libs == NULL)
    l_loaded_libs = g_hash_table_new(g_str_hash, g_str_equal);
  int i;
  char *p;
  LValue *path;
  closure = l_closure_root(closure);
  for(i=0; i<args->core.list->len; i++) {
    path = l_list_get(args, i);
    l_assert_is(path, L_STR_TYPE, "Path for require must be a string.", closure);
    p = path->core.str->str;
    // TODO search in cwd, then in lib/extra
    if(!g_hash_table_lookup_extended(l_loaded_libs, p, NULL, NULL)) {
      l_eval_path(p, closure);
      g_hash_table_insert(l_loaded_libs, p, p);
    }
  }
  return args;
}
