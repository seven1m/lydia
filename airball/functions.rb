module Airball
  module Functions
    def build_functions
      @scope["out"] = Function.new(["expr"]) do |scope|
        @scope["stdout"].write scope["expr"].eval(scope).to_s + "\n"
      end

      @scope["+"] = Function.new(["left", "right"]) do |scope, left, right|
        Int.new(left.val + right.val)
      end

      @scope["-"] = Function.new(["left", "right"]) do |scope, left, right|
        Int.new(left.val - right.val)
      end

      @scope["*"] = Function.new(["left", "right"]) do |scope, left, right|
        Int.new(left.val * right.val)
      end

      @scope["/"] = Function.new(["left", "right"]) do |scope, left, right|
        Int.new(left.val / right.val)
      end
    end
  end
end
