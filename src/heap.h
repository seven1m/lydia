#ifndef HEAP_H
#define HEAP_H

#include <glib.h>

typedef GPtrArray LHeap;

void   l_heap_add(LHeap*, LValue*);
LHeap* l_heap_new();

#endif
