#include <airball.h>

void test_parse_empty_line(CuTest *tc) {
  GSList* ast = airball_parse("\n");
  CuAssertIntEquals(tc, 0, g_slist_length(ast));
  g_slist_free(ast);
}

void test_parse_int(CuTest *tc) {
  GSList* ast = airball_parse("3");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, INT_TYPE, n->type);
  CuAssertIntEquals(tc, 3, n->value.i);
  g_slist_free(ast);
}

void test_parse_syntax_error(CuTest *tc) {
  GSList* ast = airball_parse("[error}");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, ERR_TYPE, n->type);
  CuAssertStrEquals(tc, "[error}", n->value.e);
  g_slist_free(ast);
}

void test_parse_comment(CuTest *tc) {
  GSList* ast = airball_parse("# this is a comment\n"
                              "1 # inline with code\n"
                              "  # not left-aligned");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, INT_TYPE, n->type);
  g_slist_free(ast);
}

CuSuite* parser_test_suite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_parse_empty_line);
  SUITE_ADD_TEST(suite, test_parse_int);
  SUITE_ADD_TEST(suite, test_parse_syntax_error);
  SUITE_ADD_TEST(suite, test_parse_comment);
  return suite;
}
