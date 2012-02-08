#ifndef CLOSURE_H
#define CLOSURE_H

#include <glib.h>

typedef struct LClosure {
  LHeap *heap;
  GHashTable *vars;
} LClosure;

LClosure *l_closure_new();
LClosure *l_closure_clone(LClosure *parent);
void l_closure_set(LClosure *closure, char *name, LValue *value);
void l_closure_set_funcs(LClosure *closure);
LValue *l_closure_get(LClosure *closure, char *name);

void l_inspect_closure(LClosure* closure);

#endif
