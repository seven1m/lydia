require './airball/parser'
require './airball/transform'
require './airball/classes'
require './airball/functions'
require './airball/errors'

module Airball
  class Program
    include Functions

    def initialize(source)
      @parser = Parser.new
      @transform = Transform.new
      @source = source
    end

    def build_scope
      @scope = Scope.new
      @scope["stdout"] = ""
    end

    def run
      build_scope
      build_functions
      body = @transform.apply(@parser.parse(@source))
      body.each do |expr|
        expr.eval(@scope)
      end
      @scope["stdout"]
    end
  end

end
