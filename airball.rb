require './airball/parser'
require './airball/transform'
require './airball/classes'
require './airball/functions'
require './airball/errors'

require 'pp'

module Airball
  if defined?(AIRBALL_DEBUG)
    case AIRBALL_DEBUG
    when 1
      DEBUG = [:calls]
    else
      DEBUG = [:calls, :tree]
    end
  else
    DEBUG = false
  end

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
        begin
          parsed = @parser.parse(source)
        rescue Parslet::ParseFailed => error
          puts error.message
          puts error.backtrace
          puts @parser.root.error_tree
        else
          if DEBUG && DEBUG.include?(:tree)
            puts "DEBUG: Parse Tree ----------------"
            pp parsed
            puts "------------------- End Parse Tree"
          end
        end
        body = @transform.apply(parsed)
        body[0..-2].each do |expr|
          expr.eval(@scope)
        end
        body.last.eval(@scope) # return last expression
      end
  end

end
