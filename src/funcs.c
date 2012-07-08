#include "lidija.h"

LValue *l_eval_call_node(LNode *node, LValue *func, LClosure *closure) {

  if(func == NULL) func = l_eval_var_node(node, closure);

  char *name = (node != NULL) ? node->val : "";

  LValue *value, *args_val;
  LNode *expr;
  int i;

  // create a running scope to hold arguments
  // and a reference to self (for recursion)
  // TODO this may not be necessary (can't we just do `cl=func->core.func.closure`?)
  LClosure *cl = l_closure_clone(func->core.func.closure, node);

tail_loop:

  l_debug(L_DEBUG_CALL) printf(">>> entering %s\n", name);

  if(strcmp(name, "") != 0)
    l_closure_set(cl, name, func, true);

  args_val = l_eval_call_args(node, func, closure, cl);

  if(func->core.func.ptr != NULL) {
    // native C code
    if(!setjmp(cl->jmp)) {
      value = func->core.func.ptr(args_val, cl);
    } else {
      // function called longjmp to initiate a tail call
      l_debug(L_DEBUG_CALL) printf("^^^ reached end of %s (longjmp tail call)\n", name);
      node = NULL;
      func = l_closure_get(cl, "--tail-call--");
      closure = cl;
      cl = func->core.func.closure;
      name = "";
      goto tail_loop;
    }
  } else {
    // Lidija code
    int exprc = func->core.func.exprc;
    if(exprc > 0) {
      // eval all but the last expression
      for(i=0; i<exprc-1; i++) {
        expr = func->core.func.exprs[i];
        value = l_eval_node(expr, cl);
      }
      expr = func->core.func.exprs[exprc-1];
      if(expr->type == L_CALL_TYPE) { // tail call
        l_debug(L_DEBUG_CALL) printf("^^^ reached end of %s (tail call)\n", name);
        node = expr;
        func = l_eval_var_node(node, cl);
        closure = cl;
        cl = func->core.func.closure;
        name = node->val;
        goto tail_loop;
      } else {
        value = l_eval_node(expr, cl);
      }
    } else {
      value = l_value_new(L_NIL_TYPE, cl);
    }
  }

  l_closure_free(cl);

  l_debug(L_DEBUG_CALL) printf("<<< returning from %s\n", name);

  return value;
}

LValue *l_eval_call_args(LNode *node, LValue *func, LClosure *outer_closure, LClosure *inner_closure) {
  int i, argc;
  LValue *v, *args_val, **ref;
  LNode **args;

  if(node != NULL) {
    argc = node->exprc;
    args = node->exprs;
  } else {
    argc = 0;
    args = NULL;
  }

  if(outer_closure != inner_closure) {
    // initialize all args to nil
    for(i=0; i<func->core.func.argc; i++) {
      v = l_value_new(L_NIL_TYPE, inner_closure);
      l_closure_set(inner_closure, func->core.func.args[i]->val, v, true);
    }
  }

  // setup the arguments
  args_val = l_value_new(L_LIST_TYPE, inner_closure);
  args_val->core.list = create_vector();

  LValue ***args_ref = malloc(sizeof(LValue**) * argc);

  // set all passed args
  for(i=0; i<argc; i++) {
    if(args[i]->type == L_VAR_TYPE) { // pass vars by reference
      ref = l_closure_get_ref(outer_closure, args[i]->val);
      if(ref != NULL) {
        args_ref[i] = ref;
        v = *ref;
      } else {
        // handle error
        l_eval_var_node(args[i], outer_closure);
      }
    } else { // eval as normal and set the value
      v = l_eval_node(args[i], outer_closure);
      ref = malloc(sizeof(LValue*));
      *ref = v;
      args_ref[i] = ref;
    }
    // append to 'args' variable
    vector_add(args_val->core.list, v, sizeof(v));
  }

  for(i=0; i<func->core.func.argc; i++) {
    l_ref_put(inner_closure->locals, func->core.func.args[i]->val, args_ref[i]);
  }

  l_closure_set(inner_closure, "args", args_val, true);

  return args_val;
}

// inserts given function as given name in given closure
void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure) {
  LValue *f = l_func_new(ptr, closure);
  l_closure_set(closure, name, f, false);
}

// creates all built-in functions in the given closure
void l_create_funcs(LClosure *closure) {
  l_insert_func("+",       l_func_add,       closure);
  l_insert_func("-",       l_func_num_sub,   closure);
  l_insert_func("*",       l_func_num_mul,   closure);
  l_insert_func("/",       l_func_num_div,   closure);
  l_insert_func("->",      l_func_list_get,  closure);
  l_insert_func("str",     l_func_str,       closure);
  l_insert_func("|",       l_func_str_split, closure);
  l_insert_func("out",     l_func_out,       closure);
  l_insert_func("if",      l_func_if,        closure);
  l_insert_func("while",   l_func_while,     closure);
  l_insert_func("count",   l_func_count,     closure);
  l_insert_func("rest",    l_func_rest,      closure);
  l_insert_func("==",      l_func_eq,        closure);
  l_insert_func("!=",      l_func_neq,       closure);
  l_insert_func(">=",      l_func_num_gte,   closure);
  l_insert_func("<=",      l_func_num_lte,   closure);
  l_insert_func(">",       l_func_num_gt,    closure);
  l_insert_func("<",       l_func_num_lt,    closure);
  l_insert_func("&&",      l_func_and,       closure);
  l_insert_func("||",      l_func_or,        closure);
  l_insert_func("require", l_func_require,   closure);
  l_insert_func("type",    l_func_type,      closure);
}

// sets misc global vars
void l_create_globals(LClosure *closure) {
  l_closure_set(closure, "nil",   l_value_new(L_NIL_TYPE,   closure), false);
  l_closure_set(closure, "false", l_value_new(L_FALSE_TYPE, closure), false);
  l_closure_set(closure, "true",  l_value_new(L_TRUE_TYPE,  closure), false);
}

// loads the core library
void l_load_lib(LClosure *closure) {
  // FIXME use absolute paths
#if L_SKIP_LIB == 0
  l_eval_path("lib/core/list.lid", closure);
  l_eval_path("lib/core/math.lid", closure);
#endif
}
