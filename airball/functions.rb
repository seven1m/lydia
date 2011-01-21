module Airball
  module Functions
    def build_functions
      @scope["out"] = Function.new(["expr"]) do |scope|
        scope["stdout"] << scope["expr"].eval(scope).to_s + "\n"
      end

      @scope["+"] = Function.new(["left", "right"]) do |scope|
        scope["left"].eval(scope) + scope["right"].eval(scope)
      end

      @scope["-"] = Function.new(["left", "right"]) do |scope|
        scope["left"].eval(scope) - scope["right"].eval(scope)
      end

      @scope["*"] = Function.new(["left", "right"]) do |scope|
        scope["left"].eval(scope) * scope["right"].eval(scope)
      end

      @scope["/"] = Function.new(["left", "right"]) do |scope|
        scope["left"].eval(scope) / scope["right"].eval(scope)
      end
    end
  end
end
