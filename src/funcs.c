#include "lidija.h"

LValue *l_call_func(char *name, int argc, LNode **args, LValue *func, LClosure *closure) {
  l_debug(L_DEBUG_CALL) {
    printf(">>> entering %s\n", name);
  }
  LValue *value;
  int i;
  LValue *v, **ref, **argsRef;

  // create a running scope to hold arguments
  // and a reference to self (for recursion)
  LClosure *cl = l_closure_clone(func->core.func.closure, closure);
  if(strcmp(name, "") != 0)
    l_closure_set(cl, name, func, true);

  // initialize all args to nil
  for(i=0; i<func->core.func.argc; i++) {
    v = l_value_new(L_NIL_TYPE, cl);
    l_closure_set(cl, func->core.func.args[i]->val, v, true);
  }

  // setup the arguments
  // FIXME: why do this ourselves -- why not pass a LValue* to l_closure_set ???
  argsRef = GC_MALLOC(sizeof(LValue*));
  *argsRef = l_value_new(L_LIST_TYPE, cl);
  (*argsRef)->core.list = create_vector();
  l_ref_put(cl->locals, "args", argsRef);

  // set all passed args
  for(i=0; i<argc; i++) {
    // FIXME this seems too kludgy
    if(args[i]->type == L_VAR_TYPE) {
      if((ref = l_closure_get_ref(closure, args[i]->val))) {
        v = *ref;
      } else {
        char buf[255];
        v = l_value_new(L_ERR_TYPE, closure);
        snprintf(buf, 254, "%s not found", args[i]->val);
        v->core.str = make_stringbuf(buf);
        l_handle_error(v, closure);
      }
      if(i < func->core.func.argc) {
        l_ref_put(cl->locals, func->core.func.args[i]->val, ref);
      }
    } else {
      v = l_eval_node(args[i], closure); // use calling closure
      if(i < func->core.func.argc) {
        l_closure_set(cl, func->core.func.args[i]->val, v, true);
      }
    }
    vector_add((*argsRef)->core.list, &v, sizeof(&v));
  }

  if(func->core.func.ptr != NULL) {
    // native C code
    value = func->core.func.ptr(*argsRef, cl);
  } else {
    // Lidija code
    int exprc = func->core.func.exprc;
    for(i=0; i<exprc; i++) {
      value = l_eval_node(func->core.func.exprs[i], cl);
    }
    if(exprc == 0) {
      value = l_value_new(L_NIL_TYPE, cl);
    }
  }

  l_closure_free(cl);

  l_debug(L_DEBUG_CALL) {
    printf("<<< returning from %s\n", name);
  }

  return value;
}

// inserts given function as given name in given closure
void l_insert_func(char *name, struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure) {
  LValue *f = l_func_new(ptr, closure);
  l_closure_set(closure, name, f, false);
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
  l_insert_func("while", l_func_while, closure);
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
