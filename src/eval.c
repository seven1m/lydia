#include "lidija.h"

void l_eval_node_iter(gpointer node, gpointer closure) {
#if L_DEBUG_HEAP == 1
  LValue *value = l_eval_node((LNode*)node, (LClosure*)closure);
  char buf[255] = "";
  printf("%s\n", l_inspect(value, buf, 255));
  printf("%d item(s) in the closure\n", l_closure_size((LClosure*)closure));
  l_inspect_closure(closure);
  puts("");
#else
  l_eval_node((LNode*)node, (LClosure*)closure);
#endif
}

LValue *l_eval_node(LNode *node, LClosure *closure) {
  LValue *value;
  switch(node->type) {
    case L_ERR_TYPE:
      value = l_eval_error_node(node, closure);
      l_handle_error(value, closure);
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
  if(strchr(node->val, '\\')) {
    value->core.str = g_string_new("");
    int i, len = strlen(node->val);
    char c[] = " ";
    for(i=0; i<len; i++) {
      if(node->val[i] == '\\' && i < len-1) {
        i++;
        switch(node->val[i]) {
          case 'a' : c[0] = '\a'; break;
          case 'b' : c[0] = '\b'; break;
          case 'f' : c[0] = '\f'; break;
          case 'n' : c[0] = '\n'; break;
          case 'r' : c[0] = '\r'; break;
          case 't' : c[0] = '\t'; break;
          case 'v' : c[0] = '\v'; break;
          case '\'': c[0] = '\''; break;
          case '"' : c[0] = '"' ; break;
          case '\\': c[0] = '\\'; break;
          case '?' : c[0] = '?' ; break;
        }
      } else {
        c[0] = node->val[i];
      }
      g_string_append(value->core.str, c);
    }
  } else {
    value->core.str = g_string_new(node->val);
  }
  return value;
}

LValue *l_eval_error_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_ERR_TYPE, closure);
  value->core.str = g_string_new(node->val);
  return value;
}

LValue *l_eval_assign_node(LNode *node, LClosure *closure) {
  LValue *value = l_eval_node(node->exprs[0], closure);
  l_closure_set(closure, node->val, value, false);
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
    g_array_insert_val(value->core.list, i, v);
  }
  return value;
}

LValue *l_eval_func_node(LNode *node, LClosure *closure) {
  LValue *value = l_value_new(L_FUNC_TYPE, closure);
  value->core.func.ptr = NULL;
  value->core.func.closure = l_closure_clone(closure, NULL);
  if(node->exprs[0]) {
    value->core.func.argc = node->exprs[0]->exprc;
    value->core.func.args = node->exprs[0]->exprs;
  } else {
    value->core.func.argc = 0;
  }
  value->core.func.exprc = node->exprs[1]->exprc;
  value->core.func.exprs = node->exprs[1]->exprs;
  return value;
}

LValue *l_eval_call_node(LNode *node, LClosure *closure) {
  LValue *value;
  LValue *func = l_closure_get(closure, node->val);
  if(func->type == L_FUNC_TYPE) {
    value = l_call_func(node->val, node->exprc, node->exprs, func, closure);
  } else {
    value = func; // error
  }
  return value;
}

void l_eval(const char *source, LClosure *closure) {
  LAst *ast = l_parse(source);
  if(closure == NULL) {
    closure = l_closure_new();
    l_closure_set_funcs(closure);
    g_slist_foreach(ast, l_eval_node_iter, closure);
    free(closure);
    free(ast);
  } else {
    g_slist_foreach(ast, l_eval_node_iter, closure);
  }
}

void l_eval_path(const char *filename, LClosure *closure) {
  // open file
  FILE *fp = fopen(filename, "r");
  if(fp == NULL) {
    printf("An error occurred while opening the file %s.\n", filename);
    exit(1);
  }

  // read source
  GString *source = g_string_new("");
  char buf[1024];
  while(fgets(buf, 1024, fp)) {
    g_string_append(source, buf);
  }
  fclose(fp);

  // eval source
  l_eval(source->str, closure);
}

char *l_inspect(LValue *value, char *buf, int bufLen) {
  char *repr;
  switch(value->type) {
    case L_ERR_TYPE:
      snprintf(buf, bufLen-1, "<Err: %s>", value->core.str->str);
      break;
    case L_NUM_TYPE:
      repr = l_str(value);
      snprintf(buf, bufLen-1, "<Num: %s>", repr);
      free(repr);
      break;
    case L_STR_TYPE:
      snprintf(buf, bufLen-1, "<Str: %s>", value->core.str->str);
      break;
    case L_LIST_TYPE:
      if(value->core.list->len > 0) {
        repr = l_str(l_list_get(value, 0));
        snprintf(buf, bufLen-1, "<List with %d item(s), first=%s>", value->core.list->len, repr);
        free(repr);
      } else {
        snprintf(buf, bufLen-1, "<List with 0 item(s)>");
      }
      break;
    case L_FUNC_TYPE:
      if(value->core.func.ptr != NULL) {
        snprintf(buf, bufLen-1, "<Func Ptr with %d arg(s) and %d expr(s)>", value->core.func.argc, value->core.func.exprc);
      } else {
        snprintf(buf, bufLen-1, "<Func with %d arg(s) and %d expr(s)>", value->core.func.argc, value->core.func.exprc);
      }
      break;
    case L_TRUE_TYPE:
      snprintf(buf, bufLen-1, "<true>");
      break;
    case L_FALSE_TYPE:
      snprintf(buf, bufLen-1, "<false>");
      break;
    case L_NIL_TYPE:
      snprintf(buf, bufLen-1, "<nil>");
      break;
    default:
      snprintf(buf, bufLen-1, "unable to inspect element type %d", value->type);
  }
  return buf;
}
