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
    when 2
      DEBUG = [:calls, :scope]
    else
      DEBUG = [:calls, :scope, :tree]
    end
  else
    DEBUG = false
  end

  class Program
    include Functions

    def initialize(source=nil, args=[])
      @parser = Parser.new
      @transform = Transform.new
      build_scope(args)
      self.source = source
    end

    def source=(s)
      if @source = s
        build_functions
        load_library
      end
    end

    attr_accessor :scope

    def run
      execute(parse(@source))
    end

    private

      def build_scope(args)
        @store = Store.new
        @scope = Closure.new(@store)
        @scope["pargs"] = args      # TODO implement as Airball::List
        @scope["stdout"] = $stdout  # TODO implement as Airball::File
      end

      def load_library
        unless @lib
          # cache the library parse tree so subsequent test runs don't have to reload it
          source = File.read(File.expand_path("../airball/library.ball", __FILE__))
          @lib = parse(source)
        end
        execute(@lib)
      end

      def parse(source)
        begin
          parsed = @parser.parse(source)
        rescue Parslet::ParseFailed => error
          puts error.message
          puts error.backtrace
          if DEBUG && DEBUG.include?(:tree)
            puts @parser.root.error_tree
          end
        else
          if DEBUG && DEBUG.include?(:tree)
            puts "DEBUG: Parse Tree ----------------"
            pp parsed
            puts "------------------- End Parse Tree"
          end
        end
        @transform.apply(parsed)
      end

      def execute(body)
        body[0..-2].each do |expr|
          expr.eval(@scope)
        end
        body.last.eval(@scope) # return last expression
      end
  end

end
