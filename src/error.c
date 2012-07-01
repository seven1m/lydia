#include "lidija.h"

void l_handle_error(LValue *error, LNode *node, LClosure *closure) {
  // TODO if error handling available, then do something nice
  // else...
  printf("Error: %s\n", error->core.str->str);
  if(node != NULL) {
    puts(l_describe_stack_frame(node));
  }
  l_print_stack(closure);
  exit(1);
}

void l_print_stack(LClosure *closure) {
  while(closure != NULL) {
    if(closure->node != NULL) {
      puts(l_describe_stack_frame(closure->node));
    }
    closure = closure->parent;
  }
}

char *l_describe_stack_frame(LNode *node) {
  char buf[255] = "";
  snprintf(buf, 254, "  line %d in %s", node->line_no, node->source_file);
  stringbuf *desc = make_stringbuf(buf);
  switch(node->type) {
    case L_CALL_TYPE:
      buffer_concat(desc, ", in call to '");
      buffer_concat(desc, node->val);
      buffer_concat(desc, "'");
      break;
    default:
      break;
  }
  return desc->str;
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
