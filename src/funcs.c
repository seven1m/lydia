#include "lidija.h"

LValue *l_func_if(LValue *args, LClosure *closure) {
  LValue *value;
  LValue *cond = l_list_get(args, 0);
  LValue *true_expr = l_list_get(args, 1);
  LValue *false_expr = l_list_get(args, 2);
  // if false
  if(cond->type == L_NIL_TYPE ||
    (cond->type == L_FALSE_TYPE) ||
    (cond->type == L_NUM_TYPE && mpz_cmp_si(cond->core.num, 0) == 0) ||
    (cond->type == L_STR_TYPE && strcmp(cond->core.str->str, "") == 0) ||
    (cond->type == L_LIST_TYPE && cond->core.list->len == 0)) {
    value = (false_expr->type == L_FUNC_TYPE) ? l_call_func("", 0, NULL, false_expr, closure) : false_expr;
  } else { // true
    value = (true_expr->type == L_FUNC_TYPE) ? l_call_func("", 0, NULL, true_expr, closure) : true_expr;
  }
  return value;
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

  value = l_eval_func_body(func, argsList, closure);

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
  l_insert_func("+", l_func_num_add, closure);
  l_insert_func("-", l_func_num_sub, closure);
  l_insert_func("*", l_func_num_mul, closure);
  l_insert_func("/", l_func_num_div, closure);
  l_insert_func("->", l_func_list_get, closure);
  l_insert_func("str", l_func_str, closure);
  l_insert_func("out", l_func_out, closure);
  l_insert_func("if", l_func_if, closure);
  l_insert_func("==", l_func_num_eq, closure);
  l_insert_func("map", l_func_map, closure);
}
