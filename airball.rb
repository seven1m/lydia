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
      load_library
    end

    attr_accessor :scope

    def build_scope(args)
      @scope = Scope.new
      @scope["pargs"] = args      # TODO implement as Airball::List
      @scope["stdout"] = $stdout  # TODO implement as Airball::File
    end

    def load_library
      lib_source = File.read(File.expand_path("../airball/library.ball", __FILE__))
      execute(lib_source)
    end

    def run
      execute(@source)
    end

    private

      def execute(source)
        body = @transform.apply(@parser.parse(source))
        body[0..-2].each do |expr|
          expr.eval(@scope)
        end
        body.last.eval(@scope) # return last expression
      end
  end

end
