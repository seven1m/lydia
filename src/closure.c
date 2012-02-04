#include <stdlib.h>
#include "airball.h"

// creates and initializes an empty closure
airb_closure* airb_closure_new() {
  airb_closure* closure = malloc(sizeof(airb_closure));
  closure->heap = airb_heap_new();
  closure->vars = g_hash_table_new(g_str_hash, g_str_equal);
  return closure;
}

// creates a new closure from the given parent closure
airb_closure* airb_closure_clone(airb_closure* parent) {
  airb_closure* closure = malloc(sizeof(airb_closure));
  closure->heap = parent->heap;
  // TODO clone, not ref copy
  // TODO increment ref_counts in heap
  closure->vars = parent->vars;
  return closure;
}

// sets a key in the closure
void airb_closure_set(airb_closure* closure, char* name, airb_value* value) {
  g_hash_table_insert(closure->vars, name, value);
  // TODO maybe increment ref_counts?
}

// gets a key in the closure
airb_value* airb_closureget(airb_closure* closure, char* name) {
  return g_hash_table_lookup(closure->vars, name);
}
