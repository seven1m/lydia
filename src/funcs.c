#include "lidija.h"

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

bool l_to_bool(LValue *cond) {
  return (cond->type == L_NIL_TYPE ||
         (cond->type == L_FALSE_TYPE) ||
         (cond->type == L_NUM_TYPE && mpz_cmp_si(cond->core.num, 0) == 0) ||
         (cond->type == L_STR_TYPE && strcmp(cond->core.str->str, "") == 0) ||
         (cond->type == L_LIST_TYPE && cond->core.list->len == 0));
}

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


LValue *l_func_require(LValue *args, LClosure *closure) {
  int i;
  LValue *path;
  closure = l_closure_root(closure);
  for(i=0; i<args->core.list->len; i++) {
    path = l_list_get(args, i);
    l_assert_is(path, L_STR_TYPE, "Path for require must be a string.", closure);
    // TODO search in cwd, then in lib/extra
    l_eval_path(path->core.str->str, closure);
  }
  return args;
}

LValue *l_call_func(char *name, int argc, LNode **args, LValue *func, LClosure *closure) {
  LValue *value;
  int i, len;
  LValue *v;

  // create a scope to hold arguments and func self-ref
  LClosure *cl = l_closure_clone(func->core.func.closure);
  l_closure_set(cl, name, func);

  // setup the arguments
  LValue *argsList = l_value_new(L_LIST_TYPE, cl);
  l_closure_set(cl, "args", argsList);
  len = max(func->core.func.argc, argc);
  argsList->core.list = g_array_sized_new(false, false, sizeof(LValue*), len);

  // initialize all args to nil
  for(i=0; i<func->core.func.argc; i++) {
    v = l_value_new(L_NIL_TYPE, cl);
    g_array_insert_val(argsList->core.list, i, v);
  }

  // set all passed args
  for(i=0; i<argc; i++) {
    v = l_eval_node(args[i], closure); // use calling closure
    v->ref_count++;
    g_array_insert_val(argsList->core.list, i, v);
  }

  value = l_eval_func_body(func, argsList, cl);

  // TODO: free(cl);
  return value;
}

LValue *l_eval_func_body(LValue *func, LValue *args, LClosure *closure) {
  LValue *value;
  int i;

  for(i=0; i<args->core.list->len; i++) {
    if(i < func->core.func.argc) {
      l_closure_set(closure, func->core.func.args[i]->val, g_array_index(args->core.list, LValue*, i));
    }
  }

  if(func->core.func.ptr != NULL) {
    // native C code
    value = func->core.func.ptr(args, closure);
  } else {
    // Lidija code
    int i, exprc = func->core.func.exprc;
    for(i=0; i<exprc; i++) {
      value = l_eval_node(func->core.func.exprs[i], closure);
    }
    if(exprc == 0) {
      value = l_value_new(L_NIL_TYPE, closure);
    }
  }
  return value;
}

// inserts given function as given name in given closure
void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure) {
  LValue *f = l_func_new(ptr, closure);
  g_hash_table_insert(closure->vars, name, f);
}

// creates all built-in functions in the given closure
void l_create_funcs(LClosure *closure) {
  l_insert_func("+", l_func_add, closure);
  l_insert_func("-", l_func_num_sub, closure);
  l_insert_func("*", l_func_num_mul, closure);
  l_insert_func("/", l_func_num_div, closure);
  l_insert_func("->", l_func_list_get, closure);
  l_insert_func("str", l_func_str, closure);
  l_insert_func("out", l_func_out, closure);
  l_insert_func("if", l_func_if, closure);
  l_insert_func("count", l_func_count, closure);
  l_insert_func("first", l_func_first, closure);
  l_insert_func("rest", l_func_rest, closure);
  l_insert_func("==", l_func_eq, closure);
  l_insert_func("!=", l_func_neq, closure);
  l_insert_func(">=", l_func_num_gte, closure);
  l_insert_func("<=", l_func_num_lte, closure);
  l_insert_func(">", l_func_num_gt, closure);
  l_insert_func("<", l_func_num_lt, closure);
  l_insert_func("&&", l_func_and, closure);
  l_insert_func("||", l_func_or, closure);
  l_insert_func("require", l_func_require, closure);
  l_insert_func("type", l_func_type, closure);
}

// sets misc global vars
void l_create_globals(LClosure *closure) {
  g_hash_table_insert(closure->vars, "nil", l_value_new(L_NIL_TYPE, closure));
  g_hash_table_insert(closure->vars, "true", l_value_new(L_TRUE_TYPE, closure));
  g_hash_table_insert(closure->vars, "false", l_value_new(L_FALSE_TYPE, closure));
}

// loads the core library
void l_load_lib(LClosure *closure) {
  // FIXME use absolute paths
  l_eval_path("lib/core/list.lid", closure);
  l_eval_path("lib/core/math.lid", closure);
}
