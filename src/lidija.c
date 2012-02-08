#include <stdio.h>
#include "lidija.h"

int main(int argc, char **argv) {
  l_eval("4\n"
         "year = 2012\n"
         "'hello world'\n"
         "year");
  return 0;
}
