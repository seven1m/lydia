#include "lidija.h"

static void l_inspect_closure_iter(gpointer key, gpointer val, gpointer user_data);
static void l_clone_closure_ref(gpointer name, gpointer val, gpointer closure);

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
  closure->vars = g_hash_table_new(g_str_hash, g_str_equal);
  g_hash_table_foreach(parent->vars, l_clone_closure_ref, closure);
  return closure;
}

static void l_clone_closure_ref(gpointer name, gpointer val, gpointer closure) {
  g_hash_table_insert(((LClosure*)closure)->vars, name, val);
  ((LValue*)val)->ref_count++;
}

// sets a key in the closure
void l_closure_set(LClosure *closure, char *name, LValue *value) {
  value->ref_count++;
  g_hash_table_insert(closure->vars, name, value);
}

// sets all built-in functions
// should be used after l_closure_new
void l_closure_set_funcs(LClosure *closure) {
  l_create_funcs(closure);
  l_load_lib(closure);
}

// gets a key in the closure
LValue *l_closure_get(LClosure *closure, char *name) {
  LValue *value = g_hash_table_lookup(closure->vars, name);
  if(value == NULL) {
    char buf[255];
    value = l_value_new(L_ERR_TYPE, closure);
    snprintf(buf, 254, "%s not found", name);
    value->core.str = g_string_new(buf);
    l_handle_error(value, closure);
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
  printf("  %s = %s\n", (char*)key, l_inspect(val, buf, 255));
}

