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
end
