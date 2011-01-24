require File.expand_path('../../airball', __FILE__)
require 'test/unit'

class TransformTest < Test::Unit::TestCase
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

  def test_call
    t = transform("foo 2 * 3\nbar 4\nout x\nout (bar)")
    assert_instance_of Airball::Call, t[0]
    assert_instance_of Airball::Call, t[1]
    assert_instance_of Airball::Call, t[2]
    assert_instance_of Airball::Call, t[3]
    assert_instance_of Airball::Call, t[3].args[0]
  end

  def test_assign
    t = transform("foo = 1")
    assert_instance_of Airball::Assign, t[0]
  end

  def test_func
    t = transform("{ foo 1 2 }")
    assert_instance_of Airball::Function, t[0]
  end

  def test_func_with_args
    t = transform("[x y] { foo x y }")
    assert_instance_of Airball::Function, t[0]
    assert_equal ["x", "y"], t[0].args
  end

  def test_string
    t = transform(%("Hello world"))
    assert_instance_of Airball::Str, t[0]
    assert_equal "Hello world", t[0].val
  end

  def test_list
    t = transform('[1 "x" y 2 * 3]')
    assert_instance_of Airball::List, t[0]
    assert_equal 4, t[0].vals.length
    assert_equal ['Airball::Int', 'Airball::Str', 'Airball::Var', 'Airball::Call'],
                 t[0].vals.map { |v| v.class.name }
  end

  def test_range
    t = transform("1..10")
    assert_instance_of Airball::Rng, t[0]
    assert_instance_of Airball::Int, t[0].first
    assert_instance_of Airball::Int, t[0].last
  end
end
