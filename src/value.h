#ifndef VALUE_H
#define VALUE_H

// forward declare LClosure
struct LClosure;
typedef struct LClosure LClosure;

#define L_VALUE_BUILTIN_FLAG (1 << 0)

// structure for representing dynamic,
// evaluated objects
typedef struct LValue {
  int ref_count;
  int flags;
  enum LNodeType type;
  union {
    mpz_t num;
    GString* str;
    GArray* list;
    struct {
      struct LValue* (*ptr)(struct LValue*, LClosure*);
      LClosure *closure;
      int argc;
      LNode **args;
      int exprc;
      LNode **exprs;
    } func;
  } core;
} LValue;

LValue *l_value_new(enum LNodeType type, LClosure *closure);
LValue *l_value_new_builtin(enum LNodeType type, LClosure *closure);
LValue *l_func_new(struct LValue* (*ptr)(struct LValue*, LClosure*), LClosure *closure);
void l_value_free(LValue *value);
bool l_value_builtin(LValue *value);

#endif
