require File.expand_path('../../lib/airball', __FILE__)
require 'test/unit'

PROGRAM = Airball::Program.new

class StringTest < Test::Unit::TestCase
  def execute(source)
    @program = PROGRAM
    @program.source = source
    @program.scope["stdout"] = out = StringIO.new
    @program.run
    out.string
  end

  def test_str
    output = execute("out (str 1)
                      out (str [2 3 4])")
    assert_equal "1\n2\n3\n4\n", output
  end
end
