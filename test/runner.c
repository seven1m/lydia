#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmockery.h>
#include <airball.h>

void test_parse_int(void **state) {
  GSList* ast = airball_parse("3");
  node* n;
  assert_int_equal(1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  assert_int_equal(INT_TYPE, n->type);
  assert_int_equal(3, n->value.i);
}

int main(int argc, char* argv[]) {
    const UnitTest tests[] = {
        unit_test(test_parse_int),
    };
    return run_tests(tests);
}
