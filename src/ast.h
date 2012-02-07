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

/* structure for representing static,
   literal nodes in the source tree */
typedef struct LNode {
  enum LNodeType type;
  char *val;
  int exprc;
  struct LNode **exprs;
} LNode;

/* A Func node is represented like so:
   val = NULL
   exprc = 2 (always)
   exprs[0] = LNode
              type = L_LIST_TYPE
              exprc = # of arguments
              exprs = array of LNodes, one for each arg
   exprs[1] = LNode
              type = L_LIST_TYPE
              exprc = # of exprs
              exprs = array of LNodes, one for each expr */

#endif
