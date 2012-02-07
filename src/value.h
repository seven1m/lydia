#ifndef VALUE_H
#define VALUE_H

// structure for representing dynamic,
// evaluated objects
typedef struct LValue {
  int ref_count;
  enum LNodeType type;
  union {
    int num;
  } value;
} LValue;

LValue *l_value_new(LNode *node);

#endif
