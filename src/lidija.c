#include <stdio.h>
#include "lidija.h"

int main(int argc, char **argv) {
  //l_eval("2 + 2\ny = 4\n8\n2..3\n{ x * y }\nfoo\n[1 2 3]\n'hello world'");
  l_eval("4\n"
         "year = 2012\n"
         "'hello world'");
  return 0;
}
