#ifndef ERROR_H
#define ERROR_H

void l_handle_error(LValue *error, LClosure *closure);
void l_die(char *message, LClosure *closure);
void l_assert_is(LValue *value, enum LNodeType type, char *error, LClosure *closure);

#endif
