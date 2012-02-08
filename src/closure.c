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
  value->ref_count++;
  g_hash_table_insert(closure->vars, name, value);
}

// gets a key in the closure
LValue *l_closure_get(LClosure *closure, char *name) {
  return g_hash_table_lookup(closure->vars, name);
}

static void l_inspect_closure_iter(gpointer key, gpointer val, gpointer user_data) {
  char buf[255] = "";
  printf("%s = %s\n", key, l_inspect(val, buf, 255));
}

// prints keys and vals in a closure
void l_inspect_closure(LClosure* closure) {
  printf("Closure contents:\n");
  g_hash_table_foreach(closure->vars, l_inspect_closure_iter, NULL);
  puts("");
}
