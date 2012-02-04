#ifndef HEAP_H
#define HEAP_H

#include <glib.h>

typedef GPtrArray airb_heap;

void       airb_heap_add(airb_heap*, airb_value*);
airb_heap* airb_heap_new();

#endif
