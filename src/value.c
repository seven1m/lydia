#include <glib.h>
#include "airball.h"

airb_value* airb_value_new(airb_node* node) {
  airb_value* value = malloc(sizeof(airb_value));
  value->node = node;
  value->ref_count = 0;
  return value;
}
