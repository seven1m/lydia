#ifndef ERROR_H
#define ERROR_H

void l_handle_error(LValue *error, LNode *node, LClosure *closure);
void l_print_stack(LClosure *closure);
char *l_describe_stack_frame(LNode *node);
void l_die(char *message, LClosure *closure);
void l_assert_is(LValue *value, enum LNodeType type, char *error, LClosure *closure);

#endif
