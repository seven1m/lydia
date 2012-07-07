#ifndef CLOSURE_H
#define CLOSURE_H

typedef struct LClosure {
  hashmap_p vars;
  hashmap_p locals;
  struct LClosure *parent;
  bool cloneable;
  LNode *node;
} LClosure;

LClosure *l_closure_new(LNode *node);
LClosure *l_closure_clone(LClosure *parent, LNode *node);
void l_clone_vars(hashmap_p from, hashmap_p to);
void l_closure_free(LClosure *closure);
LClosure *l_closure_root(LClosure *closure);
LValue **l_ref_get(hashmap_p hash, char *name);
void l_ref_put(hashmap_p hash, char *name, LValue **ref);
void l_closure_set(LClosure *closure, char *name, LValue *value, bool local);
void l_closure_set_funcs(LClosure *closure);
LValue *l_closure_get(LClosure *closure, char *name);
LValue **l_closure_get_ref(LClosure *closure, char *name);
LValue **l_closure_pass_by_ref(LNode *node, char *name, LClosure *outer_closure, LClosure *inner_closure);

void l_inspect_closure(LClosure* closure);
int l_closure_size(LClosure *closure);

#endif
