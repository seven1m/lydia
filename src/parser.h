#ifndef PARSER_H
#define PARSER_H

#define YY_INPUT(b, r, ms) (r = yy_input(b, ms))
#define YYSTYPE void*

typedef list_p LAst;

const char *yy_input_ptr;
int yy_input_len;
int yy_input(char *buf, int max_size);

#define STACK_LEN 1024
#define MAX_ARG_COUNT 255

LNode *l_stack[STACK_LEN][MAX_ARG_COUNT];
int l_stack_count[STACK_LEN];
extern int l_stackp;

void l_stack_push();
void l_stack_add(LNode *n);
LNode **l_stack_pop();

char *yytos(char *yytext, int yyleng);

LNode *l_create_num_node(char *num);
LNode *l_create_str_node(char *str);
LNode *l_create_rng_node(LNode *first, LNode *last);
LNode *l_create_var_node(char *name);
LNode *l_create_assign_node(char *name, LNode *expr);
LNode *l_create_call_node(char *name, int argc, LNode **args);
LNode *l_create_list_node(int itemc, LNode **items);
LNode *l_create_func_node(LNode *args, int exprc, LNode **exprs);
LNode *l_create_err_node(char *error, int line);

int yy_input(char *buf, int max_size);
LAst l_parse(const char *source);

LAst L_AST;

#define l_ast_add_node(n) list_add(L_AST, (n), sizeof(LNode))

#endif
