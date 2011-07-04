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

void test_parse_double_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("\"foo\"");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, STR_TYPE, n->type);
  CuAssertStrEquals(tc, "foo", n->value.s);
  g_slist_free(ast);
}

void test_parse_single_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("'bar'");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, STR_TYPE, n->type);
  CuAssertStrEquals(tc, "bar", n->value.s);
  g_slist_free(ast);
}

void test_parse_nested_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("\"\\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\\"\"\n"
                              "'\"But what?\"'");
  node* n;
  CuAssertIntEquals(tc, 2, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, STR_TYPE, n->type);
  CuAssertStrEquals(tc, "\\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\\"", n->value.s);
  n = g_slist_nth_data(ast, 1);
  CuAssertIntEquals(tc, STR_TYPE, n->type);
  CuAssertStrEquals(tc, "\"But what?\"", n->value.s);
  g_slist_free(ast);
}

void test_parse_range_with_int(CuTest *tc) {
  GSList* ast = airball_parse("1..10");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, RNG_TYPE, n->type);
  CuAssertIntEquals(tc, INT_TYPE, n->value.r->first->type);
  CuAssertIntEquals(tc, 1, n->value.r->first->value.i);
  CuAssertIntEquals(tc, INT_TYPE, n->value.r->last->type);
  CuAssertIntEquals(tc, 10, n->value.r->last->value.i);
  g_slist_free(ast);
}

void test_parse_range_with_var(CuTest *tc) {
  GSList* ast = airball_parse("1..x");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, RNG_TYPE, n->type);
  CuAssertIntEquals(tc, INT_TYPE, n->value.r->first->type);
  CuAssertIntEquals(tc, 1, n->value.r->first->value.i);
  CuAssertIntEquals(tc, VAR_TYPE, n->value.r->last->type);
  CuAssertStrEquals(tc, "x", n->value.r->last->value.v->name);
  g_slist_free(ast);
}

void test_parse_range_with_call(CuTest *tc) {
  GSList* ast = airball_parse("3..(x + 12)");
  node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, RNG_TYPE, n->type);
  CuAssertIntEquals(tc, INT_TYPE, n->value.r->first->type);
  CuAssertIntEquals(tc, 3, n->value.r->first->value.i);
  CuAssertIntEquals(tc, CALL_TYPE, n->value.r->last->type);
  CuAssertStrEquals(tc, "+", n->value.r->last->value.c->name);
  CuAssertIntEquals(tc,  2,  n->value.r->last->value.c->argc);
  CuAssertIntEquals(tc, VAR_TYPE, n->value.r->last->value.c->args[0]->type);
  CuAssertStrEquals(tc, "x", n->value.r->last->value.c->args[0]->value.v->name);
  CuAssertIntEquals(tc, INT_TYPE, n->value.r->last->value.c->args[1]->type);
  CuAssertIntEquals(tc,  12, n->value.r->last->value.c->args[1]->value.i);
  g_slist_free(ast);
}

CuSuite* parser_test_suite() {
  CuSuite* suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_parse_empty_line);
  SUITE_ADD_TEST(suite, test_parse_int);
  SUITE_ADD_TEST(suite, test_parse_syntax_error);
  SUITE_ADD_TEST(suite, test_parse_comment);
  SUITE_ADD_TEST(suite, test_parse_double_quoted_string);
  SUITE_ADD_TEST(suite, test_parse_single_quoted_string);
  SUITE_ADD_TEST(suite, test_parse_nested_quoted_string);
  SUITE_ADD_TEST(suite, test_parse_range_with_int);
  SUITE_ADD_TEST(suite, test_parse_range_with_var);
  SUITE_ADD_TEST(suite, test_parse_range_with_call);
  return suite;
}
