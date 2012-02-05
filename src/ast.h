#ifndef AST_H
#define AST_H

enum LNodeType { l_err_type, l_num_type, l_str_type, l_var_type, l_list_type, l_range_type, l_func_type, l_call_type, l_assign_type };

typedef struct LNode {
  enum LNodeType type;
  union {
    int                      num;
    char*                    str;
    char*                    err;
    char*                    var;
    struct {
      int count;
      struct LNode** items; } list;
    struct {
      struct LNode* first;
      struct LNode* last;   } range;
    struct {
      struct LNode* args;
      int exprc;
      struct LNode** exprs; } func;
    struct {
      char* name;
      int argc;
      struct LNode** args;  } call;
    struct {
      char* name;
      struct LNode* expr;   } assign;
  } value;
} LNode;

#endif
