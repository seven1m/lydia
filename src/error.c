#include "lidija.h"

void l_handle_error(LValue *error, LNode *node, LClosure *closure) {
  // TODO if error handling available, then do something nice
  // else...
  printf("Error: %s\n", error->core.str->str);
  if(node != NULL) {
    printf("  line %d in %s\n", node->line_no, node->source_file);
  }
  exit(1);
}

void l_die(char *message, LClosure *closure) {
  LValue *value = l_value_new(L_ERR_TYPE, closure);
  value->core.str = make_stringbuf(message);
  // TODO not sure if this is what I want
  l_handle_error(value, NULL, closure);
}

void l_assert_is(LValue *value, enum LNodeType type, char *error, LClosure *closure) {
  if(!value || value->type != type) {
    l_die(error, closure);
  }
}
