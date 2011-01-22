require File.expand_path('../../airball', __FILE__)
require 'test/unit'

class EvalTest < Test::Unit::TestCase
  def execute(body)
    @program = Airball::Program.new(body)
    @program.scope["stdout"] = out = StringIO.new
    @program.run
    out.string
  end

  def test_out
    expected = "6\n9\n"
    actual = execute("out 2 * 3\nout 9")
    assert_equal expected, actual
  end

  def test_assign
    execute("foo = 1")
    #assert_instance_of Airball::Int, @program.scope["foo"]
    assert_equal 1, @program.scope["foo"]
  end

end
