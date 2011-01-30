require File.expand_path('../../lib/airball', __FILE__)
require 'test/unit'

class ParserTest < Test::Unit::TestCase
  def setup
    @parser = Airball::Parser.new
  end

  def parse(body)
    @parser.parse(body)
  end

  def test_empty_line
    expected = "\n"
    actual = parse("\n")
    assert_equal expected, actual
  end

  def test_comment
    expected = [
      {:assign => {:name => "x",
                   :val => {:integer => "1"}}}
    ]
    actual = parse("# this is a comment
                    x = 1 # inline with code
                    # not left-aligned")
    assert_equal expected, actual
  end

  def test_int
    expected = [
      {:integer => "2"}
    ]
    actual = parse("2")
    assert_equal expected, actual
  end

  def test_string
    expected = [
      {:string => "\\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\\""},
      {:string => '"But what?"'},
    ]
    actual = parse(%("\\"OK, now I'm supposed to say, 'Hmm, that's interesting, but... ', then you say...\\""
                     '"But what?"'))
    assert_equal expected, actual
  end

  def test_range
    expected = [
      {:range => {:first => {:integer => "1"}, :last => {:integer => "10"}}},
      {:range => {:first => {:integer => "5"}, :last => {:var => "x"}}},
      {:range => {:first => {:integer => "3"}, :last => {:op => {:left => {:var => "x"},
                                                                 :symbol => "+",
                                                                 :right => {:integer => "12"}}}}}
    ]
    actual = parse("1..10
                    5..x
                    3..(x + 12)")
    assert_equal expected, actual
  end

  def test_list
    expected = [
      {:list => [{:integer => "1"},
                 {:string => "string"},
                 {:var => "x"},
                 {:op => {:left => {:integer => "3"},
                          :symbol => "*",
                          :right => {:integer => "5"}}}]},
      {:op => {:left => {:list => [{:integer => "1"},
                                   {:integer => "2"},
                                   {:integer => "3"}]},
               :symbol => "+",
               :right => {:list => [{:integer => "4"},
                                    {:integer => "5"},
                                    {:integer => "6"}]}}},
      {:list => ""},
      {:list => [{:integer => "1"},
                 {:integer => "2"},
                 {:integer => "3"}]}
    ]
    actual = parse('[1 "string" x 3 * 5]
                    [1 2 3] + [4 5 6]
                    []
                    [1
                     2
                     3]')
    assert_equal expected, actual
  end

  def test_op
    expected = [
      {:op => {:left => {:var => "x"}, :symbol => "*", :right => {:integer => "3"}}}
    ]
    actual = parse("x * 3")
    assert_equal expected, actual
  end

  def test_non_assign_symbols
    expected = [
      {:op => {:left => {:integer => "1"}, :symbol => "==", :right => {:integer => "1"}}}
    ]
    actual = parse("1 == 1")
    assert_equal expected, actual
  end

  def test_simple_call
    expected = [
      {:call => {:name => "foo", :args => [{:integer => "1"}, {:integer => "2"}]}},
      {:op   => {:left => {:var => "x"}, :symbol => "*", :right => {:integer => "3"}}}
    ]
    actual = parse("foo 1 2\nx * 3")
    assert_equal expected, actual
  end

  def test_multiline_call
    expected = [
      {:call => {:name => "foo", :args => [{:integer => "1"},
                                           {:integer => "2"}]}},
      {:call => {:name => "bar", :args => [{:integer => "3"},
                                           {:list => ""},
                                           {:func => {:body => [{:integer => "4"}]}}]}}
    ]
    actual = parse("foo 1,
                        2
                    bar 3,
                        [],
                        { 4 }")
    assert_equal expected, actual
  end

  def test_complex_call
    expected = [
      {:call => {:name => "foo",
                 :args => [{:op => {:left   => {:integer => "2"},
                                    :symbol => "*",
                                    :right  => {:integer => "3"}}}]}},
      {:call => {:name => "bar",
                 :args => [{:var => "baz"},
                           {:op => {:left   => {:integer => "4"},
                                    :symbol => "-",
                                    :right  => {:integer => "5"}}},
                           {:integer => "9"}]}}
    ]
    actual = parse("foo 2 * 3\nbar baz 4 - 5 9")
    assert_equal expected, actual
  end

  def test_no_arg_call
    expected = [
      {:call => {:name => "foo"}},
      {:call => {:name => "bar", :args => [{:call => {:name => "foo"}}]}}
    ]
    actual = parse("(foo)\nbar (foo)")
  end

  def test_nested_call
    expected = [
      {:op => {:left => {:op => {:left => {:var => "x"},
                                 :symbol => "*",
                                 :right => {:integer => "3"}}},
               :symbol => "-",
               :right => {:integer => "1"}}},
      {:call => {:name => "foo",
                 :args => [{:op => {:left => {:var => "bar"},
                                    :symbol => "&&",
                                    :right => {:var => "baz"}}},
                           {:integer => "1"},
                           {:integer => "2"}]}}
    ]
    actual = parse("(x * 3) - 1\n
                    foo (bar && baz) 1 2")
    assert_equal expected, actual

    expected = [
      {:op => {:left => {:var => "x"},
               :symbol => "*",
               :right => {:call => {:name => "foo",
                                    :args => [{:integer => "4"}]}}}},
    ]
    actual = parse("x * (foo 4)")
    assert_equal expected, actual

    expected = [
      {:op => {:left => {:var => "x"},
               :symbol => "*",
               :right => {:op => {:left => {:var => "x"},
                                  :symbol => "^",
                                  :right => {:op => {:left => {:var => "y"},
                                                     :symbol => "-",
                                                     :right => {:integer => "1"}}}}}}}
    ]
    actual = parse("x * (x ^ (y - 1))")
    assert_equal expected, actual
  end

  def test_assigns
    expected = [
      {:assign => {:name => "foo",
                   :val => {:integer => "3"}}},
      {:assign => {:name => "bar",
                   :val => {:var => "foo"}}},
      {:assign => {:name => "baz",
                   :val => {:call => {:name => "bap",
                                      :args => [{:integer => "3"}]}}}},
      {:assign => {:name => "^",
                   :val => {:var => "func"}}}
    ]
    actual = parse("foo = 3\nbar = foo\nbaz = bap 3\n^ = func")
    assert_equal expected, actual
  end

  def test_func
    expected = [
      {:func => {:body => [{:call => {:name => "foo",
                                      :args => [{:integer => "1"},
                                                {:integer => "2"}]}}]}}
    ]
    actual = parse("{ foo 1 2 }")
    assert_equal expected, actual
    actual = parse("{\n foo 1 2\n }")
    assert_equal expected, actual
  end

  def test_func_with_multiple_args
    expected = [
      {:func => {:args => [{:arg => "x"},
                           {:arg => "y"}],
                 :body => [{:call => {:name => "foo",
                                      :args => [{:var => "x"},
                                                {:var => "y"}]}}]}}
    ]
    actual = parse("[x y] { foo x y }")
    assert_equal expected, actual
    actual = parse("[ x y ]{\n foo x y\n }")
    assert_equal expected, actual
  end

  def test_func_with_single_arg
    expected = [
      {:func => {:args => [{:arg => "x"}],
                 :body => [{:call => {:name => "foo",
                                      :args => [{:var => "x"},
                                                {:var => "y"}]}}]}}
    ]
    actual = parse("[x] { foo x y }")
    assert_equal expected, actual
    actual = parse("[ x ]{\n foo x y\n }")
    assert_equal expected, actual
  end

  def test_assign_func
    expected = [
      {:assign => {
        :name => "foo",
        :val => {:func => {:args => [{:arg => "x"},
                                     {:arg => "y"}],
                           :body => [{:call => {:name => "bar",
                                                :args => [{:var => "x"},
                                                          {:var => "y"}]}}]}}}}
    ]
    actual = parse("foo = [x y] { bar x y }")
    assert_equal expected, actual
  end

  def test_func_return_func
    expected = [
      {:func => {:body => [{:func => {:args => [{:arg => "x"}],
                                      :body => [{:integer => "1"}]}}]}}
    ]
    actual = parse("{ [x] { 1 } }")
    assert_equal expected, actual
  end

  def test_leading_space_on_line
    expected = [
      {:var => "x"},
      {:var => "y"},
      {:var => "z"}
    ]
    actual = parse("x
                    y
                    z")
    assert_equal expected, actual
  end
end
