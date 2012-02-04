#ifndef CLOSURE_H
#define CLOSURE_H

#include <glib.h>

typedef struct airb_closure {
  airb_heap* heap;
  GHashTable* vars;
} airb_closure;

airb_closure* airb_closure_new();
airb_closure* airb_closure_clone(airb_closure*);
void          airb_closure_set(airb_closure*, char*, airb_value*);
airb_value*   airb_closure_get(airb_closure*, char*);

#endif
