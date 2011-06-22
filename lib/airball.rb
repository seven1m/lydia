%w(parser classes functions errors).each { |l| require File.expand_path("../airball/#{l}", __FILE__) }

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
      build_scope(args)
      self.source = source
    end

    def source=(s)
      if @source = s
        build_functions
        load_library
        @parsed = parse(@source)
      end
    end

    attr_accessor :scope

    def run
      execute(@parsed)
    end

    private

      def build_scope(args)
        @store = Store.new
        @scope = Closure.new(@store)
        @scope["pargs"] = List.new(args.map { |a| Str.new(a) })
        @scope["stdout"] = AirballFile.new($stdout)
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
        parsed = @parser.parse(source)
        if DEBUG && DEBUG.include?(:tree)
          puts "DEBUG: Parse Tree ----------------"
          pp parsed
          puts "------------------- End Parse Tree"
        end
        if Hash === parsed.first and error = parsed.first[:error]
          puts "Syntax error: ", parsed.inspect
          exit(1)
        end
        parsed
      end

      def execute(body)
        body[0..-2].each do |expr|
          expr.eval(@scope)
        end
        body.last.eval(@scope) if body.last # return last expression
      end
  end

end
