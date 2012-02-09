#include "lidija.h"

void l_eval_node_iter(gpointer node, gpointer closure) {
  LValue *value = l_eval_node((LNode*)node, (LClosure*)closure);
  char buf[255] = "";
  printf("%s\n", l_inspect(value, buf, 255));
  printf("%d item(s) in the heap\n", l_heap_size(((LClosure*)closure)->heap));
  l_inspect_heap(((LClosure*)closure)->heap);
  l_inspect_closure(closure);
  puts("");
}

LValue *l_eval_node(LNode *node, LClosure *closure) {
  LValue *value;
  switch(node->type) {
    case L_ERR_TYPE:
      value = l_eval_error_node(node, closure);
      break;
    case L_NUM_TYPE:
      value = l_eval_num_node(node, closure);
      break;
    case L_STR_TYPE:
      value = l_eval_string_node(node, closure);
      break;
    case L_VAR_TYPE:
      value = l_eval_var_node(node, closure);
      break;
    case L_LIST_TYPE:
      value = l_eval_list_node(node, closure);
      break;
    case L_FUNC_TYPE:
      value = l_eval_func_node(node, closure);
      break;
    case L_CALL_TYPE:
      value = l_eval_call_node(node, closure);
      break;
    case L_ASSIGN_TYPE:
      value = l_eval_assign_node(node, closure);
      break;
    default:
      printf("ERROR: unable to eval element");
  }
  return value;
}

LValue *l_eval_num_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_NUM_TYPE, closure);
  mpz_init_set_str(value->core.num, node->val, 0);
  return value;
}

LValue *l_eval_string_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_STR_TYPE, closure);
  value->core.str = g_string_new(node->val);
  return value;
}

LValue *l_eval_error_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_ERR_TYPE, closure);
  value->core.str = g_string_new(node->val);
  return value;
}

LValue *l_eval_assign_node(LNode *node, LClosure *closure) {
  LValue *value = l_eval_node(node->exprs[0], closure);
  l_closure_set(closure, node->val, value);
  return value;
}

LValue *l_eval_var_node(LNode *node, LClosure *closure) {
  LValue *value = l_closure_get(closure, node->val);
  return value;
}

LValue *l_eval_list_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_LIST_TYPE, closure);
  value->core.list = g_array_sized_new(false, false, sizeof(LValue*), node->exprc);
  LValue *v;
  int i;
  for(i=0; i<node->exprc; i++) {
    v = l_eval_node(node->exprs[i], closure);
    v->ref_count++;
    g_array_insert_val(value->core.list, i, v);
  }
  return value;
}

LValue *l_eval_func_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_FUNC_TYPE, closure);
  value->core.func.ptr = NULL;
  value->core.func.closure = l_closure_clone(closure);
  value->core.func.argc = node->exprs[0]->exprc;
  value->core.func.args = node->exprs[0]->exprs;
  value->core.func.exprc = node->exprs[1]->exprc;
  value->core.func.exprs = node->exprs[1]->exprs;
  return value;
}

LValue *l_eval_call_node(LNode *node, LClosure *closure) {
  LValue *value;
  LValue *func = l_closure_get(closure, node->val);
  if(func->type == L_FUNC_TYPE) {
    int i, len;
    LValue *v;

    // create a scope to hold arguments and func self-ref
    LClosure *cl = l_closure_clone(func->core.func.closure);
    l_closure_set(cl, node->val, func);

    // setup the arguments
    LValue *args = l_value_new(L_LIST_TYPE, cl);
    l_closure_set(cl, "args", args);
    len = max(func->core.func.argc, node->exprs[0]->exprc);
    args->core.list = g_array_sized_new(false, false, sizeof(LValue*), len);

    // initialize all args to nil
    for(i=0; i<func->core.func.argc; i++) {
      v = l_value_new(L_NIL_TYPE, cl);
      g_array_insert_val(args->core.list, i, v);
    }

    // set all passed args
    for(i=0; i<node->exprc; i++) {
      v = l_eval_node(node->exprs[i], closure); // use calling closure
      v->ref_count++;
      g_array_insert_val(args->core.list, i, v);
      if(i < func->core.func.argc) {
        l_closure_set(cl, func->core.func.args[i]->val, v);
      }
    }

    // eval func body
    if(func->core.func.ptr != NULL) {
      // native C code
      value = func->core.func.ptr(args, cl);
    } else {
      // Lidija code
      int exprc = func->core.func.exprc;
      for(i=0; i<exprc; i++) {
        value = l_eval_node(func->core.func.exprs[i], cl);
      }
      if(exprc == 0) {
        value = l_value_new(L_NIL_TYPE, closure);
      }
    }
    // TODO: free(cl);
  } else {
    value = func; // error
  }
  return value;
}

void l_eval(const char *source) {
  LAst *ast = l_parse(source);
  LClosure *closure = l_closure_new();
  l_closure_set_funcs(closure);
  g_slist_foreach(ast, l_eval_node_iter, closure);
  free(closure);
  free(ast);
}

char *l_inspect(LValue *value, char *buf, int bufLen) {
  char *repr;
  switch(value->type) {
    case L_ERR_TYPE:
      snprintf(buf, bufLen-1, "<Err: %s>", value->core.str->str);
      break;
    case L_NUM_TYPE:
      repr = l_str(value, NULL);
      snprintf(buf, bufLen-1, "<Num: %s>", repr);
      free(repr);
      break;
    case L_STR_TYPE:
      snprintf(buf, bufLen-1, "<Str: %s>", value->core.str->str);
      break;
    case L_LIST_TYPE:
      snprintf(buf, bufLen-1, "<List with %d item(s)>", value->core.list->len);
      break;
    case L_FUNC_TYPE:
      if(value->core.func.ptr != NULL) {
        snprintf(buf, bufLen-1, "<Func Ptr>");
      } else {
        snprintf(buf, bufLen-1, "<Func with %d arg(s) and %d expr(s)>", value->core.func.argc, value->core.func.exprc);
      }
      break;
    default:
      sprintf(buf, "unable to inspect element");
  }
  return buf;
}
