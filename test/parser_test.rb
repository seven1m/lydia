require File.expand_path('../../lib/airball', __FILE__)
require 'test/unit'

class ParserTest < Test::Unit::TestCase
  include Airball

  def setup
    @parser = Airball::Parser.new
  end

  def parse(string)
    @parser.parse(string)
  end

  def test_empty_line
    assert_equal [], parse("\n")
  end

  def test_syntax_error
    actual = parse("[error}")
    assert_instance_of Errors::SyntaxError, actual[0]
  end

  def test_int
    expected = [
      Int.new(2)
    ]
    actual = parse("2")
    assert_equal expected, actual
  end

  def test_comment
    expected = [
      Assign.new("x", Int.new(1))
    ]
    actual = parse("# this is a comment
                    x = 1 # inline with code
                    # not left-aligned")
    assert_equal expected, actual
  end

  def test_string
    expected = [
      Str.new("foo")
    ]
    actual = parse(%("foo"))
    assert_equal expected, actual

    expected = [
      Str.new("bar")
    ]
    actual = parse(%('bar'))
    assert_equal expected, actual

    expected = [
      Str.new("\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\""),
      Str.new('"But what?"')
    ]
    actual = parse(%("\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\""
                     '"But what?"'))
    assert_equal expected, actual
  end

  def test_range
    expected = [
      Rng.new(Int.new(1), Int.new(10))
    ]
    actual = parse("1..10")
    assert_equal expected, actual

    expected = [
      Rng.new(Int.new(5), Var.new("x"))
    ]
    actual = parse("5..x")
    assert_equal expected, actual

    expected = [
      Rng.new(Int.new(3), Call.new("+", [Var.new("x"), Int.new(12)]))
    ]
    actual = parse("3..(x + 12)")
    assert_equal expected, actual
  end

  def test_list
    expected = [
      List.new([Int.new(1),
                Str.new("string"),
                Var.new("x"),
                Call.new("*", [Int.new(3),
                               Int.new(5)])])
    ]
    actual = parse('[1 "string" x 3 * 5]')
    assert_equal expected, actual
  end

  def test_list_with_comma_separators
    expected = [
      List.new([Int.new(1),
                Str.new("string"),
                Var.new("x"),
                Call.new("*", [Int.new(3),
                               Int.new(5)])])
    ]
    actual = parse('[1, "string", x, 3 * 5]')
    assert_equal expected, actual
  end

  def test_list_as_operand
    expected = [
      Call.new("+", [List.new([Int.new(1),
                               Int.new(2),
                               Int.new(3)]),
                     List.new([Int.new(4),
                               Int.new(5),
                               Int.new(6)])])
    ]
    actual = parse('[1 2 3] + [4 5 6]')
    assert_equal expected, actual
  end

  def test_empty_list
    expected = [
      List.new([])
    ]
    actual = parse('[]')
    assert_equal expected, actual
  end

  def test_multiline_list
    expected = [
      List.new([Int.new(1),
                Int.new(2),
                Int.new(3)])
    ]
    actual = parse('[1
                     2
                     3]')
    assert_equal expected, actual
  end

  def test_nested_list
    expected = [
      List.new([Int.new(1),
                List.new([Var.new("x"),
                          Var.new("y")]),
                Int.new(2)])
    ]
    actual = parse("[1 [x y] 2]")
    assert_equal expected, actual
  end

  def test_op
    expected = [
      Call.new("*", [Var.new("x"), Int.new(3)])
    ]
    actual = parse("x * 3")
    assert_equal expected, actual
  end

  def test_non_assign_symbols
    expected = [
      Call.new("==", [Int.new(1), Int.new(1)])
    ]
    actual = parse("1 == 1")
    assert_equal expected, actual
  end

  def test_simple_call
    expected = [
      Call.new("foo", [Int.new(1), Int.new(2)])
    ]
    actual = parse("foo 1 2")
    assert_equal expected, actual

    expected = [
      Call.new("*", [Var.new("x"), Int.new(3)])
    ]
    actual = parse("x * 3")
    assert_equal expected, actual
  end

  def test_multiline_call
    expected = [
      Call.new("foo", [Int.new(1), Int.new(2)])
    ]
    actual = parse("foo 1,
                        2")
    assert_equal expected, actual

    expected = [
      Call.new("bar", [Int.new(3),
                       List.new([]),
                       Function.new([], [Int.new(4)])])
    ]
    actual = parse("bar 3,
                        [],
                        { 4 }")
    assert_equal expected, actual
  end

  def test_complex_call
    expected = [
      Call.new("foo", [Call.new("*", [Int.new(2),
                                      Int.new(3)])])
    ]
    actual = parse("foo 2 * 3")
    assert_equal expected, actual

    expected = [
      Call.new("bar", [Var.new("baz"),
                       Call.new("-", [Int.new(4),
                                      Int.new(5)]),
                       Int.new(9)])
    ]
    actual = parse("bar baz 4 - 5 9")
    assert_equal expected, actual
  end

  def test_no_arg_call
    expected = [
      Call.new("foo", []),
      Call.new("bar", [Call.new("foo", [])])
    ]
    actual = parse("(foo)\nbar (foo)")
    assert_equal expected, actual
  end

  def test_symbols
    assert_equal [Call.new("+", [Var.new("x"), Int.new(3)])], parse("x + 3")
    assert_equal [Call.new("-", [Var.new("x"), Int.new(3)])], parse("x - 3")
    assert_equal [Call.new("*", [Var.new("x"), Int.new(3)])], parse("x * 3")
    assert_equal [Call.new("/", [Var.new("x"), Int.new(3)])], parse("x / 3")
  end

  def test_nested_call
    expected = [
      Call.new("foo", [Var.new("x"),
                       Call.new("bar", [Var.new("y")]),
                       Var.new("z")])
    ]
    actual = parse("foo x (bar y) z")
    assert_equal expected, actual

    expected = [
      Call.new("-", [Call.new("*", [Var.new("x"), Int.new(3)]),
                     Int.new(1)]),
      Call.new("foo", [Call.new("&&", [Var.new("bar"),
                                       Var.new("baz")]),
                       Int.new(1),
                       Int.new(2)])
    ]
    actual = parse("(x * 3) - 1
                    foo (bar && baz) 1 2")
    assert_equal expected, actual

    expected = [
      Call.new("*", [Var.new("x"),
                     Call.new("foo", [Int.new(4)])])
    ]
    actual = parse("x * (foo 4)")
    assert_equal expected, actual

    expected = [
      Call.new("*", [Var.new("x"),
                     Call.new("^", [Var.new("x"),
                                    Call.new("-", [Var.new("y"),
                                                   Int.new(1)])])])
    ]
    actual = parse("x * (x ^ (y - 1))")
    assert_equal expected, actual
  end

  def test_assigns
    expected = [
      Assign.new("foo", Int.new(3)),
      Assign.new("bar", Var.new("foo")),
      Assign.new("baz", Call.new("bap", [Int.new(3)])),
      Assign.new("^",   Var.new("func"))
    ]
    actual = parse("foo = 3\nbar = foo\nbaz = bap 3\n^ = func")
    assert_equal expected, actual
  end

  def test_func
    expected = [
      Function.new([], [Call.new("foo", [Int.new(1), Int.new(2)])])
    ]
    actual = parse("{ foo 1 2 }")
    assert_equal expected, actual

    actual = parse("{\n foo 1 2\n }")
    assert_equal expected, actual

    expected = [
      Function.new([], [Assign.new("x", Int.new(1)),
                        Call.new("*", [Var.new("x"),
                                       Int.new(3)])])
    ]
    actual = parse("{ x = 1
                      x * 3 }")
    assert_equal expected, actual
  end

  def test_func_with_multiple_args
    expected = [
      Function.new(["x", "y"], [Call.new("foo", [Var.new("x"),
                                                 Var.new("y")])])
    ]
    actual = parse("[x y] { foo x y }")
    assert_equal expected, actual

    actual = parse("[ x y ]{\n foo x y\n }")
    assert_equal expected, actual
  end

  def test_func_with_single_arg
    expected = [
      Function.new(["x"], [Call.new("foo", [Var.new("x"),
                                            Var.new("y")])])
    ]
    actual = parse("[x] { foo x y }")
    assert_equal expected, actual

    actual = parse("[ x ]{\n foo x y\n }")
    assert_equal expected, actual
  end

  def test_recursion
    expected = [
      Assign.new("fact", Function.new(["x"],
                                      [Call.new("if", [Call.new("==", [Var.new("x"),
                                                                       Int.new(1)]),
                                                       Var.new("x"),
                                                       Function.new([], [Call.new("*", [Var.new("x"),
                                                                                        Call.new("fact", [Call.new("-", [Var.new("x"), Int.new(1)])])])])])]))
    ]
    actual = parse("fact = [x] { if x == 1,
                                    x,
                                    { x * (fact x - 1) } }")
    assert_equal expected, actual
  end

  def test_assign_func
    expected = [
      Assign.new("foo", Function.new(["x", "y"], [Call.new("bar", [Var.new("x"), Var.new("y")])]))
    ]
    actual = parse("foo = [x y] { bar x y }")
    assert_equal expected, actual
  end

  def test_func_return_func
    expected = [
      Function.new([], [Function.new(["x"], [Int.new(1)])])
    ]
    actual = parse("{ [x] { 1 } }")
    assert_equal expected, actual
  end

  def test_leading_space_on_line
    expected = [
      Var.new("x"),
      Var.new("y"),
      Var.new("z")
    ]
    actual = parse("x
                    y
                    z")
    assert_equal expected, actual
  end

  # this is no longer an issue, and slow to test
  #def test_large_input
    #assert_nothing_raised do
      #parse("24 " * 100000)
    #end
  #end
end
