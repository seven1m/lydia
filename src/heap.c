#include "lidija.h"

void l_heap_add(LHeap *heap, LValue *value) {
  g_ptr_array_add(heap, value);
}

LHeap *l_heap_new() {
  return g_ptr_array_new();
}

int l_heap_size(LHeap *heap) {
  int counter = 0;
  g_ptr_array_foreach(heap, l_heap_size_iter, &counter);
  return counter;
}

void l_heap_size_iter(gpointer value, gpointer counter) {
  ((*(int*)counter))++;
}
