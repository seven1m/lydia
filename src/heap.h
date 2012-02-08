#ifndef HEAP_H
#define HEAP_H

#include <glib.h>

// granted, not a true heap
// we only use this to free memory, not allocate it
typedef GPtrArray LHeap;

void l_heap_add(LHeap *heap, LValue *value);
LHeap *l_heap_new();
int l_heap_size(LHeap *heap);
void l_inspect_heap(LHeap* heap);

#endif
