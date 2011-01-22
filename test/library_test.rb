require File.expand_path('../../airball', __FILE__)
require 'test/unit'

class LibraryTest < Test::Unit::TestCase
  def execute(body)
    @program = Airball::Program.new(body)
    @program.run.to_s
  end

  def test_exponentiation
    assert_equal "16", execute("2 ^ 4")
  end
end
