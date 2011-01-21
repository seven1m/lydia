require File.expand_path('../../airball', __FILE__)
require 'test/unit'

class AirballTest < Test::Unit::TestCase
  def setup
    @parser = Airball::Parser.new
    @transform = Airball::Transform.new
  end

  def parse(body)
    @parser.parse(body)
  end

  def transform(body)
    @transform.apply(parse(body))
  end

  def execute(body)
    Airball::Program.new(body).run
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

  def test_transform
    t = transform("foo 2 * 3\nbar 4")
    assert Airball::Call === t[0]
    assert Airball::Call === t[1]
  end

  def test_execute
    expected = "6\n9\n"
    actual = execute("out 2 * 3\nout 4 + 5")
    assert_equal expected, actual
  end

end
