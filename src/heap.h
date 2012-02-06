#ifndef HEAP_H
#define HEAP_H

#include <glib.h>

typedef GPtrArray LHeap;

void l_heap_add(LHeap *heap, LValue *value);
LHeap *l_heap_new();

#endif
