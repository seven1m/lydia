#ifndef AST_H
#define AST_H

enum LNodeType {
  L_ERR_TYPE,
  L_NUM_TYPE,
  L_STR_TYPE,
  L_VAR_TYPE,
  L_LIST_TYPE,
  L_RANGE_TYPE,
  L_FUNC_TYPE,
  L_CALL_TYPE,
  L_ASSIGN_TYPE
};

// structure for representing static,
// literal nodes in the source tree
typedef struct LNode {
  enum LNodeType type;
  union {
    int                      num;
    char                    *str;
    char                    *err;
    char                    *var;
    struct {
      int count;
      struct LNode **items; } list;
    struct {
      struct LNode *first;
      struct LNode *last;   } range;
    struct {
      struct LNode *args;
      int exprc;
      struct LNode **exprs; } func;
    struct {
      char *name;
      int argc;
      struct LNode **args;  } call;
    struct {
      char *name;
      struct LNode *expr;   } assign;
  } value;
} LNode;

#endif
