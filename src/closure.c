#include "lidija.h"

/*static void l_inspect_closure_iter(gpointer key, gpointer val, gpointer user_data);*/

// creates and initializes an empty closure
LClosure *l_closure_new() {
  LClosure *closure = GC_MALLOC(sizeof(LClosure));
  closure->vars = create_hashmap();
  closure->locals = create_hashmap();
  closure->parent = NULL;
  closure->cloneable = true;
  return closure;
}

// creates a new closure from the given parent closure
LClosure *l_closure_clone(LClosure *parent, LClosure *evaling) {
  if(!parent->cloneable) return parent;
  LClosure *closure = GC_MALLOC(sizeof(LClosure));
  closure->vars = create_hashmap();
  closure->locals = create_hashmap();
  closure->parent = parent;
  closure->cloneable = true;
  // copy vars from function closure
  l_clone_vars(parent->vars, closure->vars);
  l_clone_vars(parent->locals, closure->locals);
  // TODO remove this??? not sure why I ever thought I needed this...
  //if(evaling != NULL) // copy locals from evaling scope
  //  g_hash_table_foreach(evaling->locals, l_clone_closure_local_ref, closure);
  return closure;
}

void l_clone_vars(hashmap_p from, hashmap_p to) {
  int i;
  char *key;
  LValue **ref;
  for(i=0; i<from->keys->length; i++) {
    key = vector_get(from->keys, i);
    ref = *((LValue***)hashmap_get(from, key));
    hashmap_put(to, key, &ref, sizeof(&ref));
  }
}

void l_closure_free(LClosure *closure) {
  if(closure->parent == NULL) return;
}

// given a closure, returns the root closure
LClosure *l_closure_root(LClosure *closure) {
  LClosure *parent = closure;
  while(parent->parent != NULL) {
    parent = parent->parent;
  }
  return parent;
}

LValue **l_ref_get(hashmap_p hash, char *name) {
  LValue ***refref;
  if((refref = hashmap_get(hash, name))) {
    if(*refref != NULL) return *refref;
  }
  return NULL;
}

void l_ref_put(hashmap_p hash, char *name, LValue **ref) {
  hashmap_put(hash, name, &ref, sizeof(&ref));
}

// sets a key in the closure
void l_closure_set(LClosure *closure, char *name, LValue *value, bool local) {
  LValue **ref = NULL;
  if(local || (ref = l_ref_get(closure->locals, name))) {
    if(ref == NULL) {
      ref = GC_MALLOC(sizeof(LValue*));
      *ref = value;
      l_ref_put(closure->locals, name, ref);
    } else {
      *ref = value;
    }
  } else if((ref = l_ref_get(closure->vars, name))) {
    *ref = value;
  } else {
    ref = GC_MALLOC(sizeof(LValue*));
    *ref = value;
    l_ref_put(closure->vars, name, ref);
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
  LValue **ref, *value;
  if((ref = l_closure_get_ref(closure, name))) {
    return *ref;
  } else {
    char buf[255];
    value = l_value_new(L_ERR_TYPE, closure);
    snprintf(buf, 254, "%s not found", name);
    value->core.str = make_stringbuf(buf);
    l_handle_error(value, closure);
    return value;
  }
}

LValue **l_closure_get_ref(LClosure *closure, char *name) {
  LValue **ref;
  if((ref = l_ref_get(closure->locals, name)) ||
     (ref = l_ref_get(closure->vars, name))) {
    return ref;
  } else {
    return NULL;
  }
}

// prints keys and vals in a closure
void l_inspect_closure(LClosure* closure) {
  printf("--------------------\n");
  printf("Closure var contents:\n");
  //g_hash_table_foreach(closure->vars, l_inspect_closure_iter, NULL);
  printf("Closure local contents:\n");
  //g_hash_table_foreach(closure->locals, l_inspect_closure_iter, NULL);
  printf("====================\n");
}

/*static void l_inspect_closure_iter(gpointer key, gpointer ref, gpointer user_data) {*/
  /*char buf[255] = "";*/
  /*printf("  %s = %s\n", (char*)key, l_inspect_to_str(*(LValue**)ref, buf, 255));*/
/*}*/

int l_closure_size(LClosure *closure) {
  return closure->vars->size;
}

