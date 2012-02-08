#ifndef EVAL_H
#define EVAL_H

void l_eval(const char *source);
void l_eval_node(gpointer node, gpointer user_data);
LValue *l_eval_num_node(LNode *node, LClosure *closure);

char *l_inspect(LValue *value, char *buf, int bufLen);

#endif
