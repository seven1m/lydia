#include "lidija.h"

void l_heap_add(LHeap *heap, LValue *value) {
  g_ptr_array_add(heap, value);
}

LHeap *l_heap_new() {
  return g_ptr_array_new();
}

static void l_heap_size_iter(gpointer value, gpointer counter) {
  ((*(int*)counter))++;
}

int l_heap_size(LHeap *heap) {
  int counter = 0;
  g_ptr_array_foreach(heap, l_heap_size_iter, &counter);
  return counter;
}

static void l_inspect_heap_iter(gpointer val, gpointer user_data) {
  char buf[255] = "";
  printf("  %s (refcount: %d)\n", l_inspect((LValue*)val, buf, 255), ((LValue*)val)->ref_count);
}

// prints keys and vals in a closure
void l_inspect_heap(LHeap* heap) {
  printf("Heap contents:\n");
  g_ptr_array_foreach(heap, l_inspect_heap_iter, NULL);
}
