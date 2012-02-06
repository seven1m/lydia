#include <stdlib.h>
#include "lidija.h"

// creates and initializes an empty closure
LClosure *l_closure_new() {
  LClosure *closure = malloc(sizeof(LClosure));
  closure->heap = l_heap_new();
  closure->vars = g_hash_table_new(g_str_hash, g_str_equal);
  return closure;
}

// creates a new closure from the given parent closure
LClosure *l_closure_clone(LClosure *parent) {
  LClosure *closure = malloc(sizeof(LClosure));
  closure->heap = parent->heap;
  // TODO clone, not ref copy
  // TODO increment ref_counts in heap
  closure->vars = parent->vars;
  return closure;
}

// sets a key in the closure
void l_closure_set(LClosure *closure, char *name, LValue *value) {
  g_hash_table_insert(closure->vars, name, value);
  // TODO maybe increment ref_counts?
}

// gets a key in the closure
LValue *l_closure_get(LClosure *closure, char *name) {
  return g_hash_table_lookup(closure->vars, name);
}
