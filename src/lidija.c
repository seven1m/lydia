#include <stdio.h>
#include "lidija.h"

int main(int argc, char **argv) {
  l_eval("4\n"
         "l = [1 2 3]\n"
         "year = 2012\n"
         "'hello world'\n"
         "year\n"
         "x = [a b] { a * b }\n"
         "x 2 3"
         );
  return 0;
}
