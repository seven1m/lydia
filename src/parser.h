#ifndef PARSER_H
#define PARSER_H

#define YY_INPUT(b, r, ms) (r = yy_input(b, ms))
#define YYSTYPE void*

GSList* l_parse(char *);

char *yy_input_ptr;
int yy_input_len;
int yy_input(char *buf, int max_size);

#define STACK_LEN 1024
#define MAX_ARG_COUNT 255

LNode* l_stack[STACK_LEN][MAX_ARG_COUNT];
int l_stack_count[STACK_LEN];
extern int l_stackp;

void l_stack_push();
void l_stack_add(LNode*);
LNode** l_stack_pop();

char* yytos(char*, int);

LNode* l_create_int_node(char*, int);
LNode* l_create_str_node(char*, int);
LNode* l_create_rng_node(LNode*, LNode*);
LNode* l_create_var_node(char*);
LNode* l_create_assign_node(char*, LNode*);
LNode* l_create_call_node(char*, int, LNode**);
LNode* l_create_list_node(int, LNode**);
LNode* l_create_func_node(LNode*, int, LNode**);
LNode* l_create_err_node(char*);

GSList* L_AST;

#define L_ADD_NODE(n) L_AST = g_slist_append(L_AST, n)

#endif
