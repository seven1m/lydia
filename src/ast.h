#ifndef AST_H
#define AST_H

enum LNodeType {
  L_NIL_TYPE,   // 0
  L_TRUE_TYPE,  // 1
  L_FALSE_TYPE, // 2
  L_ERR_TYPE,   // 3
  L_NUM_TYPE,   // 4
  L_STR_TYPE,   // 5
  L_VAR_TYPE,   // 6
  L_LIST_TYPE,  // 7
  L_FUNC_TYPE,  // 8
  L_CALL_TYPE,  // 9
  L_ASSIGN_TYPE // 10
};

/* structure for representing static,
   literal nodes in the source tree */
typedef struct LNode {
  enum LNodeType type;
  char *val;
  int exprc;
  char *source_file;
  int line_no;
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
