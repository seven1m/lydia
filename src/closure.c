#include "lidija.h"

// creates and initializes an empty closure
LClosure *l_closure_new(LNode *node) {
  l_debug(L_DEBUG_STACK) printf("+++ creating new closure\n");
  LClosure *closure = GC_MALLOC(sizeof(LClosure));
  closure->vars = create_hashmap();
  closure->locals = create_hashmap();
  closure->parent = NULL;
  closure->cloneable = true;
  closure->node = node;
  return closure;
}

// creates a new closure from the given parent closure
LClosure *l_closure_clone(LClosure *parent, LNode *node) {
  l_debug(L_DEBUG_STACK) printf("+++ cloning closure\n");
  if(!parent->cloneable) return parent;
  LClosure *closure = GC_MALLOC(sizeof(LClosure));
  closure->vars = create_hashmap();
  closure->locals = create_hashmap();
  closure->parent = parent;
  closure->cloneable = true;
  closure->node = node;
  // copy vars from function closure
  l_clone_vars(parent->vars, closure->vars);
  l_clone_vars(parent->locals, closure->locals);
  return closure;
}

static void l_clone_var_iter(char *key, void *ref, void *to) {
  l_ref_put((hashmap_p)to, key, (LValue**)ref);
}


void l_clone_vars(hashmap_p from, hashmap_p to) {
  hashmap_each(from, l_clone_var_iter, to);
}

void l_closure_free(LClosure *closure) {
  l_debug(L_DEBUG_STACK) printf("--- freeing closure\n");
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
  LValue **ref;
  if((ref = hashmap_get(hash, name))) {
    return ref;
  }
  return NULL;
}

void l_ref_put(hashmap_p hash, char *name, LValue **ref) {
  hashmap_put(hash, name, ref, sizeof(ref));
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
  LValue **ref;
  if((ref = l_closure_get_ref(closure, name))) {
    return *ref;
  } else {
    return NULL;
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

LValue **l_closure_pass_by_ref(LNode *node, char *name, LClosure *outer_closure, LClosure *inner_closure) {
  LValue **ref = l_closure_get_ref(outer_closure, node->val);
  if(ref != NULL) {
    if(name != NULL) l_ref_put(inner_closure->locals, name, ref);
    return ref;
  } else {
    // handle error
    l_eval_var_node(node, outer_closure);
    return NULL;
  }
}

static void l_inspect_closure_iter(char *key, void *ref, void *opt) {
  char buf[255] = "";
  printf("  %s = %s\n", key, l_inspect_to_str(*(LValue**)ref, buf, 255));
}

// prints keys and vals in a closure
void l_inspect_closure(LClosure* closure) {
  printf("--------------------\n");
  printf("Closure var contents:\n");
  hashmap_each(closure->vars, l_inspect_closure_iter, NULL);
  printf("Closure local contents:\n");
  hashmap_each(closure->locals, l_inspect_closure_iter, NULL);
  printf("====================\n");
}

int l_closure_size(LClosure *closure) {
  return closure->vars->size;
}

