#ifndef HEAP_H
#define HEAP_H

#include <glib.h>

typedef GPtrArray LHeap;

void l_heap_add(LHeap *heap, LValue *value);
LHeap *l_heap_new();
int l_heap_size(LHeap *heap);
void l_heap_size_iter(gpointer value, gpointer counter);

#endif
