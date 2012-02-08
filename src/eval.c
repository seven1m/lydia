#include "lidija.h"

void l_eval_node_iter(gpointer node, gpointer closure) {
  LValue *value = l_eval_node((LNode*)node, (LClosure*)closure);
  char buf[255] = "";
  printf("%s\n", l_inspect(value, buf, 255));
  printf("%d item(s) in the heap\n", l_heap_size(((LClosure*)closure)->heap));
  l_inspect_heap(((LClosure*)closure)->heap);
  l_inspect_closure(closure);
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
      break;
    case L_FUNC_TYPE:
      break;
    case L_CALL_TYPE:
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

void l_eval(const char *source) {
  LAst *ast = l_parse(source);
  LClosure *closure = l_closure_new();
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
      repr = mpz_get_str(NULL, 10, value->core.num);
      snprintf(buf, bufLen-1, "<Num: %s>", repr);
      free(repr);
      break;
    case L_STR_TYPE:
      snprintf(buf, bufLen-1, "<Str: %s>", value->core.str->str);
      break;
    case L_LIST_TYPE:
      //snprintf(buf, bufLen-1, "<List: %d>", node->exprc);
      break;
    case L_FUNC_TYPE:
      //snprintf(buf, bufLen-1, "<Func with %d expr(s)>", node->exprs[1]->exprc);
      break;
    case L_CALL_TYPE:
      //snprintf(buf, bufLen-1, "<Call with %d arg(s)>", node->exprc);
      break;
    case L_ASSIGN_TYPE:
      //b1 = malloc(sizeof(char) * 255);
      //l_inspect(node->exprs[0], b1, 255);
      //snprintf(buf, bufLen-1, "<Assign: %s = %s>", node->val, b1);
      //free(b1);
      break;
    default:
      sprintf(buf, "unable to inspect element");
  }
  return buf;
}
