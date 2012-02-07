#ifndef EVAL_H
#define EVAL_H

void l_eval_node(gpointer node, gpointer user_data);
void l_eval(const char *source);
char *l_inspect(LNode *node, char *buf, int bufLen);

#endif
