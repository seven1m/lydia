#ifndef PARSER_H_
#define PARSER_H_

#include <string.h>
#include <glib.h>

#define YY_INPUT(b, r, ms) (r = yy_input(b, ms))
#define YYSTYPE void*

char *yy_input_ptr;
int yy_input_len;
int yy_input(char *buf, int max_size);

#define STACK_LEN 1024
#define MAX_ARG_COUNT 255

node* stack[STACK_LEN][MAX_ARG_COUNT]; /* holds lists of data */
int stack_count[STACK_LEN];             /* number of items at each level in the stack */
int stackp = 0;                        /* position on stack */

node* rb_ary_new();

void stack_push();
void stack_add(node*);
node** stack_pop();

char* yytos(char*, int);

node* create_int_node(char*, int);
node* create_str_node(char*, int);
node* create_rng_node(node*, node*);
node* create_var_node(char*);
node* create_call_node(char*, int, node**);
node* create_list_node(int, node**);
node* create_func_node(node*, int, node**);
node* create_err_node(char*);

void add_node_to_ast(node*);

GSList* parse_ast;

#define ADD_NODE(n) parse_ast = g_slist_append(parse_ast, n)

#endif
