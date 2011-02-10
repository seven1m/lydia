module Airball
  class Parser
    def syntax_error(expr)
      Errors::SyntaxError.new(expr)
    end

    def assign(name, val)
      Assign.new(name, val)
    end

    def integer(val)
      Int.new(val.to_i)
    end

    def string(val)
      Str.new(val.gsub(/\\/, ''))
    end

    def range(first, last)
      Rng.new(first, last)
    end

    def var(name)
      Var.new(name)
    end

    def op(symbol, left, right)
      Call.new(symbol, [left, right])
    end

    def call(name, args)
      Call.new(name, args)
    end

    def func(args, body)
      Function.new(args, body)
    end

    def list(vals)
      List.new(vals)
    end

    def add(ast)
      @expressions << ast
    end
  end
end

require(File.expand_path("../../../parser.so", __FILE__))
