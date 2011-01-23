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
    assert_instance_of Airball::Int, @program.scope["foo"]
    assert_equal 1, @program.scope["foo"].val
  end

  def test_string
    output = execute(%(out 'in single quotes'
                       out "in double quotes"
                       out "mixed 'quotes'"
                       out "escaped \\"double quotes\\""))
    assert_equal "in single quotes\n" +
                 "in double quotes\n" +
                 "mixed 'quotes'\n"   +
                 "escaped \"double quotes\"\n", output
  end

  def test_list
    output = execute("out [1 'string' 3 * 5 ]")
    assert_equal "1\nstring\n15\n", output
  end

  def test_func
    output = execute("foo = { x = 2\nx * 3 }\nout (foo)")
    assert_instance_of Airball::Function, @program.scope["foo"]
    assert_equal "6\n", output
  end

  def test_func_with_args
    output = execute("foo = [x y] { x - y }\nz = foo 4 3\nout z")
    assert_instance_of Airball::Function, @program.scope["foo"]
    assert_equal "1\n", output
  end

  def test_closure
    output = execute("func = { x = 2
                               [y] { x + y }}
                      closure = (func)
                      out (closure 10)")
    assert_equal "12\n", output
  end

  def test_bool
    output = execute("out 1 == 1")
    assert_equal "true\n", output
    output = execute("out 1 == 2")
    assert_equal "false\n", output
    output = execute("out true")
    assert_equal "true\n", output
    output = execute("out false")
    assert_equal "false\n", output
  end

  def test_if
    output = execute("if 1 > 2 { out 3 } { out 4 }")
    assert_equal "4\n", output
  end

  def test_factorial
    output = execute("fact = [x] { if x == 1 x { x * (fact x - 1) } }
                      out (fact 5)")
    assert_equal "120\n", output
  end

  def test_scoping
    output = execute("x = 0
                      f = { x }
                      g = { x = 1
                            (f) }
                      out (g)
                      out x")
    assert_equal "1\n0\n", output
  end

end
