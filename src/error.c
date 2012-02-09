#include "lidija.h"

void l_handle_error(LValue *error, LClosure *closure) {
  // TODO
  printf("Error: %s\n", error->core.str->str);
  exit(1);
}
