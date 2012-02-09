#include <stdio.h>
#include "lidija.h"

bool DEBUG = true;

int main(int argc, char **argv) {
  l_eval("out 4\n"
         "l = [1 2 3]\n"
         "out (l -> 1)\n"
         "year = 2012\n"
         "out 'hello world'\n"
         "out year\n"
         "x = [a b] { a * b }\n"
         "out (x 2 3)\n"
         "out 1 ' to ' 10\n"
         "out [3 4 5]\n"
         );
  return 0;
}
