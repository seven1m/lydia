require File.expand_path('../../airball', __FILE__)
require 'test/unit'

class ParserTest < Test::Unit::TestCase
  def setup
    @parser = Airball::Parser.new
  end

  def parse(body)
    @parser.parse(body)
  end

  def test_simple_expr
    expected = [
      {:call => {:name => "foo", :args => [{:integer => "1"}, {:integer => "2"}]}},
      {:op   => {:left => {:var => "x"}, :symbol => "*", :right => {:integer => "3"}}}
    ]
    actual = parse("foo 1 2\nx * 3")
    assert_equal expected, actual
  end

  def test_complex_expr
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

  def test_assign
    expected = [
      {:assign => {:name => "foo",
                   :val => {:integer => "3"}}},
      {:assign => {:name => "bar",
                   :val => {:var => "foo"}}},
      {:assign => {:name => "baz",
                   :val => {:call => {:name => "bap",
                                      :args => [{:integer => "3"}]}}}}
    ]
    actual = parse("foo = 3\nbar = foo\nbaz = bap 3")
    assert_equal expected, actual
  end
end
