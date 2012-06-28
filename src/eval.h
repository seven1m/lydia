#ifndef EVAL_H
#define EVAL_H

void l_eval(const char *source, const char *source_file, LClosure *closure);
void l_eval_path(const char *filename, LClosure *closure);

LValue *l_eval_node(LNode *node, LClosure *closure);
LValue *l_eval_num_node(LNode *node, LClosure *closure);
LValue *l_eval_string_node(LNode *node, LClosure *closure);
LValue *l_eval_var_node(LNode *node, LClosure *closure);
LValue *l_eval_list_node(LNode *node, LClosure *closure);
LValue *l_eval_func_node(LNode *node, LClosure *closure);
LValue *l_eval_call_node(LNode *node, LClosure *closure);
LValue *l_eval_error_node(LNode *node, LClosure *closure);
LValue *l_eval_assign_node(LNode *node, LClosure *closure);

void l_inspect(LValue *value);
char *l_inspect_to_str(LValue *value, char *buf, int bufLen);

#endif
