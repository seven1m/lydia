#include <stdio.h>
#include "lidija.h"

int main(int argc, char **argv) {
  l_eval("4\n"
         "l = [1 2 3]\n"
         "year = 2012\n"
         "'hello world'\n"
         "year\n"
         "x = [a b] { a }\n"
         "x 1 2"
         );
  return 0;
}
