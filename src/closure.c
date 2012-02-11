#include "lidija.h"

static void l_inspect_closure_iter(gpointer key, gpointer val, gpointer user_data);
static void l_clone_closure_ref(gpointer name, gpointer val, gpointer closure);

// creates and initializes an empty closure
LClosure *l_closure_new() {
  LClosure *closure = malloc(sizeof(LClosure));
  closure->heap = l_heap_new();
  closure->vars = g_hash_table_new(g_str_hash, g_str_equal);
  closure->parent = NULL;
  closure->cloneable = true;
  return closure;
}

// creates a new closure from the given parent closure
LClosure *l_closure_clone(LClosure *parent) {
  if(!parent->cloneable) return parent;
  LClosure *closure = malloc(sizeof(LClosure));
  closure->heap = parent->heap;
  closure->vars = g_hash_table_new(g_str_hash, g_str_equal);
  closure->parent = parent;
  closure->cloneable = true;
  g_hash_table_foreach(parent->vars, l_clone_closure_ref, closure);
  return closure;
}

static void l_clone_closure_ref(gpointer name, gpointer val, gpointer closure) {
  g_hash_table_insert(((LClosure*)closure)->vars, name, val);
  (*((LValue**)val))->ref_count++;
}

// given a closure, returns the root closure
LClosure *l_closure_root(LClosure *closure) {
  LClosure *parent = closure;
  while(parent->parent != NULL) {
    parent = parent->parent;
  }
  return parent;
}

// sets a key in the closure
void l_closure_set(LClosure *closure, char *name, LValue *value) {
  value->ref_count++;
  LValue **v;
  if(v = g_hash_table_lookup(closure->vars, name)) {
    *v = value;
    // TODO: decrement ref_count on existing var?
  } else {
    LValue **ref = malloc(sizeof(LValue*)); // FIXME is this right?
    *ref = value;
    g_hash_table_insert(closure->vars, name, ref);
  }
}

// sets all built-in functions
// should be used after l_closure_new
void l_closure_set_funcs(LClosure *closure) {
  closure->cloneable = false;
  l_create_funcs(closure);
  l_create_globals(closure);
  l_load_lib(closure);
  closure->cloneable = true;
}

// gets a key in the closure
LValue *l_closure_get(LClosure *closure, char *name) {
  LValue **ref = g_hash_table_lookup(closure->vars, name);
  LValue *value;
  if(ref == NULL) {
    char buf[255];
    value = l_value_new(L_ERR_TYPE, closure);
    snprintf(buf, 254, "%s not found", name);
    value->core.str = g_string_new(buf);
    l_handle_error(value, closure);
  } else {
    value = *ref;
  }
  return value;
}

// prints keys and vals in a closure
void l_inspect_closure(LClosure* closure) {
  printf("Closure contents:\n");
  g_hash_table_foreach(closure->vars, l_inspect_closure_iter, NULL);
}

static void l_inspect_closure_iter(gpointer key, gpointer val, gpointer user_data) {
  char buf[255] = "";
  printf("  %s = %s\n", (char*)key, l_inspect(*(LValue**)val, buf, 255));
}

int l_closure_size(LClosure *closure) {
  return g_hash_table_size(closure->vars);
}

