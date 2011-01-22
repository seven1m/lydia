require './airball/parser'
require './airball/transform'
require './airball/classes'
require './airball/functions'
require './airball/errors'

module Airball
  class Program
    include Functions

    def initialize(source, args=[])
      @parser = Parser.new
      @transform = Transform.new
      @source = source
      build_scope(args)
      build_functions
    end

    attr_accessor :scope

    def build_scope(args)
      @scope = Scope.new
      @scope["pargs"] = args      # TODO implement as Airball::List
      @scope["stdout"] = $stdout  # TODO implement as Airball::File
    end

    def run
      body = @transform.apply(@parser.parse(@source))
      body.each do |expr|
        expr.eval(@scope)
      end
    end
  end

end
