#include <airball.h>

void test_parse_empty_line(CuTest *tc) {
  GSList* ast = airball_parse("\n");
  CuAssertIntEquals(tc, 0, g_slist_length(ast));
  g_slist_free(ast);
}

void test_parse_num(CuTest *tc) {
  GSList* ast = airball_parse("3");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, num_type, n->type);
  CuAssertIntEquals(tc, 3, n->value.num);
  g_slist_free(ast);
}

void test_parse_syntax_error(CuTest *tc) {
  GSList* ast = airball_parse("[error}");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, err_type, n->type);
  CuAssertStrEquals(tc, "[error}", n->value.err);
  g_slist_free(ast);
}

void test_parse_comment(CuTest *tc) {
  GSList* ast = airball_parse("# this is a comment\n"
                              "1 # inline with code\n"
                              "  # not left-aligned");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, num_type, n->type);
  g_slist_free(ast);
}

void test_parse_double_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("\"foo\"");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, str_type, n->type);
  CuAssertStrEquals(tc, "foo", n->value.str);
  g_slist_free(ast);
}

void test_parse_single_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("'bar'");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, str_type, n->type);
  CuAssertStrEquals(tc, "bar", n->value.str);
  g_slist_free(ast);
}

void test_parse_nested_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("\"\\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\\"\"\n"
                              "'\"But what?\"'");
  node* n;
  CuAssertIntEquals(tc, 2, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, str_type, n->type);
  CuAssertStrEquals(tc, "\\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\\"", n->value.str);
  n = g_slist_nth_data(ast, 1);
  CuAssertIntEquals(tc, str_type, n->type);
  CuAssertStrEquals(tc, "\"But what?\"", n->value.str);
  g_slist_free(ast);
}

void test_parse_range_with_num(CuTest *tc) {
  GSList* ast = airball_parse("1..10");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, range_type, n->type);
  CuAssertIntEquals(tc, num_type, n->value.range.first->type);
  CuAssertIntEquals(tc, 1, n->value.range.first->value.num);
  CuAssertIntEquals(tc, num_type, n->value.range.last->type);
  CuAssertIntEquals(tc, 10, n->value.range.last->value.num);
  g_slist_free(ast);
}

void test_parse_range_with_var(CuTest *tc) {
  GSList* ast = airball_parse("1..x");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, range_type, n->type);
  CuAssertIntEquals(tc, num_type, n->value.range.first->type);
  CuAssertIntEquals(tc, 1, n->value.range.first->value.num);
  CuAssertIntEquals(tc, var_type, n->value.range.last->type);
  CuAssertStrEquals(tc, "x", n->value.range.last->value.var);
  g_slist_free(ast);
}

void test_parse_range_with_call(CuTest *tc) {
  GSList* ast = airball_parse("3..(x + 12)");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, range_type, n->type);
  CuAssertIntEquals(tc, num_type, n->value.range.first->type);
  CuAssertIntEquals(tc, 3, n->value.range.first->value.num);
  CuAssertIntEquals(tc, call_type, n->value.range.last->type);
  CuAssertStrEquals(tc, "+", n->value.range.last->value.call.name);
  CuAssertIntEquals(tc,  2,  n->value.range.last->value.call.argc);
  CuAssertIntEquals(tc, var_type, n->value.range.last->value.call.args[0]->type);
  CuAssertStrEquals(tc, "x", n->value.range.last->value.call.args[0]->value.var);
  CuAssertIntEquals(tc, num_type, n->value.range.last->value.call.args[1]->type);
  CuAssertIntEquals(tc,  12, n->value.range.last->value.call.args[1]->value.num);
  g_slist_free(ast);
}

CuSuite* parser_test_suite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_parse_empty_line);
  SUITE_ADD_TEST(suite, test_parse_num);
  SUITE_ADD_TEST(suite, test_parse_syntax_error);
  SUITE_ADD_TEST(suite, test_parse_comment);
  SUITE_ADD_TEST(suite, test_parse_double_quoted_string);
  SUITE_ADD_TEST(suite, test_parse_single_quoted_string);
  SUITE_ADD_TEST(suite, test_parse_nested_quoted_string);
  SUITE_ADD_TEST(suite, test_parse_range_with_num);
  SUITE_ADD_TEST(suite, test_parse_range_with_var);
  SUITE_ADD_TEST(suite, test_parse_range_with_call);
  return suite;
}
