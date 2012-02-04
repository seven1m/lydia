#include "../src/airball.h"

void test_parse_empty_line(CuTest *tc) {
  GSList* ast = airball_parse("\n");
  CuAssertIntEquals(tc, 0, g_slist_length(ast));
  g_slist_free(ast);
}

void test_parse_num(CuTest *tc) {
  GSList* ast = airball_parse("3");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, num_type, n->type);
  CuAssertIntEquals(tc, 3, n->value.num);
  g_slist_free(ast);
}

void test_parse_syntax_error(CuTest *tc) {
  GSList* ast = airball_parse("[error}");
  airb_node* n;
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
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, num_type, n->type);
  g_slist_free(ast);
}

void test_parse_double_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("\"foo\"");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, str_type, n->type);
  CuAssertStrEquals(tc, "foo", n->value.str);
  g_slist_free(ast);
}

void test_parse_single_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("'bar'");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, str_type, n->type);
  CuAssertStrEquals(tc, "bar", n->value.str);
  g_slist_free(ast);
}

void test_parse_nested_quoted_string(CuTest *tc) {
  GSList* ast = airball_parse("\"\\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\\"\"\n"
                              "'\"But what?\"'");
  airb_node* n;
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
  airb_node* n;
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
  airb_node* n;
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
  airb_node* n;
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


void test_parse_list(CuTest *tc) {
  GSList* ast = airball_parse("[1 'string' x 3 * 5]");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, list_type, n->type);
  CuAssertIntEquals(tc, 4,         n->value.list.count);
  CuAssertIntEquals(tc, 1,         n->value.list.items[0]->value.num);
  CuAssertStrEquals(tc, "string",  n->value.list.items[1]->value.str);
  CuAssertStrEquals(tc, "x",       n->value.list.items[2]->value.var);
  CuAssertIntEquals(tc, call_type, n->value.list.items[3]->type);
  g_slist_free(ast);
}

void test_parse_list_with_commas(CuTest *tc) {
  GSList* ast = airball_parse("[1, 'string', x, 3 * 5]");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, list_type, n->type);
  CuAssertIntEquals(tc, 4,         n->value.list.count);
  g_slist_free(ast);
}

void test_parse_list_as_operand(CuTest *tc) {
  GSList* ast = airball_parse("[1 2 3] + [4 5 6]");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, call_type, n->type);
  CuAssertStrEquals(tc, "+",       n->value.call.name);
  CuAssertIntEquals(tc, 2,         n->value.call.argc);
  CuAssertIntEquals(tc, list_type, n->value.call.args[0]->type);
  CuAssertIntEquals(tc, list_type, n->value.call.args[1]->type);
  g_slist_free(ast);
}

void test_parse_empty_list(CuTest *tc) {
  GSList* ast = airball_parse("[]");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, list_type, n->type);
  CuAssertIntEquals(tc, 0,         n->value.list.count);
  g_slist_free(ast);
}

void test_parse_multiline_list(CuTest *tc) {
  GSList* ast = airball_parse("[1\n2\n3]");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, list_type, n->type);
  CuAssertIntEquals(tc, 3,         n->value.list.count);
  CuAssertIntEquals(tc, 1,         n->value.list.items[0]->value.num);
  CuAssertIntEquals(tc, 2,         n->value.list.items[1]->value.num);
  CuAssertIntEquals(tc, 3,         n->value.list.items[2]->value.num);
  g_slist_free(ast);
}

void test_parse_nested_list(CuTest *tc) {
  GSList* ast = airball_parse("[1 [x y] 2]");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, list_type, n->type);
  CuAssertIntEquals(tc, 3,         n->value.list.count);
  CuAssertIntEquals(tc, 1,         n->value.list.items[0]->value.num);
  CuAssertIntEquals(tc, list_type, n->value.list.items[1]->type);
  CuAssertIntEquals(tc, 2,         n->value.list.items[2]->value.num);
  g_slist_free(ast);
}

void test_parse_op(CuTest *tc) {
  GSList* ast = airball_parse("x * 3");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, call_type, n->type);
  CuAssertStrEquals(tc, "*",       n->value.call.name);
  CuAssertIntEquals(tc, 2,         n->value.call.argc);
  CuAssertStrEquals(tc, "x",       n->value.call.args[0]->value.var);
  CuAssertIntEquals(tc, 3,         n->value.call.args[1]->value.num);
  g_slist_free(ast);
}

void test_parse_non_assign_equals(CuTest *tc) {
  GSList* ast = airball_parse("1 == 1");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, call_type, n->type);
  CuAssertStrEquals(tc, "==",      n->value.call.name);
  CuAssertIntEquals(tc, 2,         n->value.call.argc);
  CuAssertIntEquals(tc, 1,         n->value.call.args[0]->value.num);
  CuAssertIntEquals(tc, 1,         n->value.call.args[1]->value.num);
  g_slist_free(ast);
}

void test_parse_simple_call(CuTest *tc) {
  GSList* ast = airball_parse("foo 1 2");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, call_type, n->type);
  CuAssertStrEquals(tc, "foo",     n->value.call.name);
  CuAssertIntEquals(tc, 2,         n->value.call.argc);
  CuAssertIntEquals(tc, 1,         n->value.call.args[0]->value.num);
  CuAssertIntEquals(tc, 2,         n->value.call.args[1]->value.num);
  g_slist_free(ast);
}

void test_parse_multiline_call(CuTest *tc) {
  GSList* ast = airball_parse("foo 1,    \n"
                              "    2     \n"
                              "bar 3,    \n"
                              "    [],   \n"
                              "    { 4 }   ");
  airb_node* n;
  CuAssertIntEquals(tc, 2, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, call_type, n->type);
  CuAssertStrEquals(tc, "foo",     n->value.call.name);
  CuAssertIntEquals(tc, 2,         n->value.call.argc);
  CuAssertIntEquals(tc, 1,         n->value.call.args[0]->value.num);
  CuAssertIntEquals(tc, 2,         n->value.call.args[1]->value.num);
  /*
  n = g_slist_nth_data(ast, 1);
  CuAssertIntEquals(tc, call_type, n->type);
  CuAssertStrEquals(tc, "bar",     n->value.call.name);
  CuAssertIntEquals(tc, 3,         n->value.call.argc);
  CuAssertIntEquals(tc, num_type,  n->value.call.args[0]->type);
  CuAssertIntEquals(tc, list_type, n->value.call.args[1]->type);
  CuAssertIntEquals(tc, func_type, n->value.call.args[2]->type);
  */
  g_slist_free(ast);
}

void test_parse_func_without_args(CuTest *tc) {
  GSList* ast = airball_parse("{ foo 1 2 }");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, func_type, n->type);
  CuAssertIntEquals(tc, 0,         n->value.func.args->value.list.count);
  CuAssertIntEquals(tc, 1,         n->value.func.exprc);
  CuAssertIntEquals(tc, call_type, n->value.func.exprs[0]->type);
  CuAssertStrEquals(tc, "foo",     n->value.func.exprs[0]->value.call.name);
  ast = airball_parse("{\n foo 1 2\n }");
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, func_type, n->type);
  CuAssertIntEquals(tc, 0,         n->value.func.args->value.list.count);
  CuAssertIntEquals(tc, 1,         n->value.func.exprc);
  CuAssertIntEquals(tc, call_type, n->value.func.exprs[0]->type);
  CuAssertStrEquals(tc, "foo",     n->value.func.exprs[0]->value.call.name);
  ast = airball_parse("{\n x = 1\n x * 3\n }");
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, func_type, n->type);
  CuAssertIntEquals(tc, 0,         n->value.func.args->value.list.count);
  CuAssertIntEquals(tc, 2,         n->value.func.exprc);
  CuAssertIntEquals(tc, call_type, n->value.func.exprs[1]->type);
  CuAssertStrEquals(tc, "*",       n->value.func.exprs[1]->value.call.name);
  g_slist_free(ast);
}

void test_parse_func_with_args(CuTest *tc) {
  GSList* ast = airball_parse("[x y]{ foo x y }");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, func_type, n->type);
  CuAssertIntEquals(tc, 2,         n->value.func.args->value.list.count);
  CuAssertIntEquals(tc, var_type,  n->value.func.args->value.list.items[0]->type);
  CuAssertStrEquals(tc, "x",       n->value.func.args->value.list.items[0]->value.var);
  CuAssertIntEquals(tc, var_type,  n->value.func.args->value.list.items[1]->type);
  CuAssertStrEquals(tc, "y",       n->value.func.args->value.list.items[1]->value.var);
  CuAssertIntEquals(tc, 1,         n->value.func.exprc);
  CuAssertIntEquals(tc, call_type, n->value.func.exprs[0]->type);
  CuAssertStrEquals(tc, "foo",     n->value.func.exprs[0]->value.call.name);
  ast = airball_parse("[ x y ] {\n foo x y\n }");
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, func_type, n->type);
  CuAssertIntEquals(tc, 2,         n->value.func.args->value.list.count);
  CuAssertIntEquals(tc, var_type,  n->value.func.args->value.list.items[0]->type);
  CuAssertStrEquals(tc, "x",       n->value.func.args->value.list.items[0]->value.var);
  CuAssertIntEquals(tc, var_type,  n->value.func.args->value.list.items[1]->type);
  CuAssertStrEquals(tc, "y",       n->value.func.args->value.list.items[1]->value.var);
  CuAssertIntEquals(tc, 1,         n->value.func.exprc);
  CuAssertIntEquals(tc, call_type, n->value.func.exprs[0]->type);
  CuAssertStrEquals(tc, "foo",     n->value.func.exprs[0]->value.call.name);
  g_slist_free(ast);
}

void test_parse_assign(CuTest *tc) {
  GSList* ast = airball_parse("x = 1");
  airb_node* n;
  CuAssertIntEquals(tc, 1, g_slist_length(ast));
  n = g_slist_nth_data(ast, 0);
  CuAssertIntEquals(tc, assign_type, n->type);
  CuAssertStrEquals(tc, "x", n->value.assign.name);
  CuAssertIntEquals(tc, 1,   n->value.assign.expr->value.num);
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
  SUITE_ADD_TEST(suite, test_parse_list);
  SUITE_ADD_TEST(suite, test_parse_list_with_commas);
  SUITE_ADD_TEST(suite, test_parse_list_as_operand);
  SUITE_ADD_TEST(suite, test_parse_empty_list);
  SUITE_ADD_TEST(suite, test_parse_multiline_list);
  SUITE_ADD_TEST(suite, test_parse_nested_list);
  SUITE_ADD_TEST(suite, test_parse_op);
  SUITE_ADD_TEST(suite, test_parse_non_assign_equals);
  SUITE_ADD_TEST(suite, test_parse_simple_call);
  SUITE_ADD_TEST(suite, test_parse_multiline_call);
  SUITE_ADD_TEST(suite, test_parse_func_without_args);
  SUITE_ADD_TEST(suite, test_parse_func_with_args);
  SUITE_ADD_TEST(suite, test_parse_assign);
  return suite;
}
