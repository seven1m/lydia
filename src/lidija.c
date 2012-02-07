#include <stdio.h>
#include "lidija.h"

int main(int argc, char **argv) {
  l_eval("2 + 2\ny = 4\n8\n2..3\n{ x * y }\n'hello world'");
  return 0;
}
