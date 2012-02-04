#ifndef PARSER_H
#define PARSER_H

#define YY_INPUT(b, r, ms) (r = yy_input(b, ms))
#define YYSTYPE void*

GSList* airball_parse(char *);

char *yy_input_ptr;
int yy_input_len;
int yy_input(char *buf, int max_size);

#define STACK_LEN 1024
#define MAX_ARG_COUNT 255

airb_node* airb_stack[STACK_LEN][MAX_ARG_COUNT];
int airb_stack_count[STACK_LEN];
extern int airb_stackp;

void airb_stack_push();
void airb_stack_add(airb_node*);
airb_node** airb_stack_pop();

char* yytos(char*, int);

airb_node* airb_create_int_node(char*, int);
airb_node* airb_create_str_node(char*, int);
airb_node* airb_create_rng_node(airb_node*, airb_node*);
airb_node* airb_create_var_node(char*);
airb_node* airb_create_assign_node(char*, airb_node*);
airb_node* airb_create_call_node(char*, int, airb_node**);
airb_node* airb_create_list_node(int, airb_node**);
airb_node* airb_create_func_node(airb_node*, int, airb_node**);
airb_node* airb_create_err_node(char*);

GSList* airb_ast;

#define AIRB_ADD_NODE(n) airb_ast = g_slist_append(airb_ast, n)

#endif
