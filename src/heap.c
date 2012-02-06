#include <glib.h>
#include "lidija.h"

void l_heap_add(LHeap *heap, LValue *value) {
  g_ptr_array_add(heap, value);
}

LHeap *l_heap_new() {
  return g_ptr_array_new();
}
