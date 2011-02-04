require File.expand_path('../../lib/airball', __FILE__)
require 'test/unit'

PROGRAM = Airball::Program.new

class EvalTest < Test::Unit::TestCase
  def execute(source)
    @program = PROGRAM
    @program.source = source
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

  def test_multi_cond_if
    output = execute("if [1 == 1 { out 'first'  }
                          2 == 3 { out 'second' }
                                 { out 'else'   }]")
    assert_equal "first\n", output
    output = execute("if [1 == 2 { out 'first'  }
                          3 == 3 { out 'second' }
                                 { out 'else'   }]")
    assert_equal "second\n", output
    output = execute("if [1 == 2 { out 'first'  }
                          2 == 3 { out 'second' }
                                 { out 'else'   }]")
    assert_equal "else\n", output
  end

  def test_and
    output = execute("if (true && false) { out 'yes' } { out 'no' }")
    assert_equal "no\n", output
    output = execute("if (true && true) { out 'yes' } { out 'no' }")
    assert_equal "yes\n", output
  end

  def test_or
    output = execute("if (true || false) { out 'yes' } { out 'no' }")
    assert_equal "yes\n", output
    output = execute("if (false || false) { out 'yes' } { out 'no' }")
    assert_equal "no\n", output
  end

  def test_recursion
    # factorial
    output = execute("fact = [x] { if x == 1,
                                      x,
                                      { x * (fact x - 1) } }
                      out (fact 5)")
    assert_equal "120\n", output
    # fibonacci sequence
    output = execute("fib = [n] { if n < 2,
                                     n,
                                     { (fib n - 1) + (fib n - 2) } }
                      out (fib 5)")
    assert_equal "5\n", output
  end

  def test_while
    output = execute("n = 5
                      while { n > 0 },
                            { out n
                              n = n - 1 }")
    assert_equal "5\n4\n3\n2\n1\n", output
  end

  def test_scoping
    assert_raise Airball::Errors::VariableNotFound do
      execute("f = { x }
               x = 1
               out (f)")
    end
    assert_nothing_raised do
      output = execute("x = 0
                        f = { x }
                        x = 1
                        out (f)")
      assert_equal "1\n", output
    end
    assert_nothing_raised do
      output = execute("x = 0
                        f = { x }
                        x = 1
                        g = { x = 2
                              (f) }
                        out (g)
                        out x")
      assert_equal "2\n2\n", output
    end
  end

end
