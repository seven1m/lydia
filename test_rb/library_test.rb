require File.expand_path('../../lib/airball', __FILE__)
require 'test/unit'

PROGRAM = Airball::Program.new unless defined?(PROGRAM)

class LibraryTest < Test::Unit::TestCase
  def execute(body)
    @program = PROGRAM
    @program.source = body
    @program.run.to_s
  end

  def test_exponentiation
    assert_equal "16", execute("2 ^ 4")
  end

  def test_map
    assert_equal "0\n2\n4\n6", execute("map [0 1 2 3],
                                            [x] { x * 2 }")
  end

  def test_join
    assert_equal "1,2,3,4", execute("join [1 2 3 4] ','")
  end
end
