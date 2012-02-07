#include "lidija.h"

void l_eval_node(gpointer node, gpointer user_data) {
  LNode *n = (LNode*)node;
  char buf[255] = "";
  printf("%s\n", l_inspect(n, buf, 255));
}

void l_eval(const char *source) {
  LAst *ast = l_parse(source);
  g_slist_foreach(ast, l_eval_node, NULL);
}

char *l_inspect(LNode *node, char *buf, int bufLen) {
  char *b1, *b2;
  switch(node->type) {
    case L_ERR_TYPE:
      snprintf(buf, bufLen-1, "<Err: %s>", node->val);
      break;
    case L_NUM_TYPE:
      snprintf(buf, bufLen-1, "<Num: %s>", node->val);
      break;
    case L_STR_TYPE:
      snprintf(buf, bufLen-1, "<Str: %s>", node->val);
      break;
    case L_VAR_TYPE:
      snprintf(buf, bufLen-1, "<Var: %s>", node->val);
      break;
    case L_LIST_TYPE:
      snprintf(buf, bufLen-1, "<List: %d>", node->exprc);
      break;
    case L_RANGE_TYPE:
      b1 = malloc(sizeof(char) * 255);
      b2 = malloc(sizeof(char) * 255);
      LNode *e1 = node->exprs[0];
      l_inspect(e1, b1, 255);
      l_inspect(node->exprs[1], b2, 255);
      snprintf(buf, bufLen-1, "<Range: %s..%s>", b1, b2);
      free(b1);
      free(b2);
      break;
    case L_FUNC_TYPE:
      snprintf(buf, bufLen-1, "<Func with %d expr(s)>", node->exprs[1]->exprc);
      break;
    case L_CALL_TYPE:
      snprintf(buf, bufLen-1, "<Call with %d arg(s)>", node->exprc);
      break;
    case L_ASSIGN_TYPE:
      b1 = malloc(sizeof(char) * 255);
      l_inspect(node->exprs[0], b1, 255);
      snprintf(buf, bufLen-1, "<Assign: %s = %s>", node->val, b1);
      free(b1);
      break;
    default:
      sprintf(buf, "unable to inspect element");
  }
  return buf;
}
