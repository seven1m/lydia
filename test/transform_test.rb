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

  def test_expr
    t = transform("foo 2 * 3\nbar 4")
    assert_instance_of Airball::Call, t[0]
    assert_instance_of Airball::Call, t[1]
  end

  def test_assign
    t = transform("foo = 1")
    assert_instance_of Airball::Assign, t[0]
  end
end
