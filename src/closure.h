#ifndef CLOSURE_H
#define CLOSURE_H

#include <glib.h>

typedef struct LClosure {
  LHeap *heap;
  GHashTable *vars;
} LClosure;

LClosure *l_closure_new();
LClosure *l_closure_clone(LClosure*);
void      l_closure_set(LClosure*, char*, LValue*);
LValue   *l_closure_get(LClosure*, char*);

#endif
