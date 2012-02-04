#include <glib.h>
#include "airball.h"

void airb_heap_add(airb_heap* heap, airb_value* value) {
  g_ptr_array_add(heap, value);
}

airb_heap* airb_heap_new() {
  return g_ptr_array_new();
}
