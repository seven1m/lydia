module Airball
  module Functions
    def build_functions
      @scope["out"] = Function.new(["expr"]) do |scope, expr|
        @scope["stdout"].write expr.to_s + "\n"
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

      @scope["true"] = True.new
      @scope["false"] = False.new

      @scope["=="] = Function.new(["left", "right"]) do |scope, left, right|
        left.val == right.val ? True.new : False.new
      end

      @scope[">"] = Function.new(["left", "right"]) do |scope, left, right|
        left.val > right.val ? True.new : False.new
      end

      @scope[">="] = Function.new(["left", "right"]) do |scope, left, right|
        left.val > right.val ? True.new : False.new
      end

      @scope["<"] = Function.new(["left", "right"]) do |scope, left, right|
        left.val < right.val ? True.new : False.new
      end

      @scope["<="] = Function.new(["left", "right"]) do |scope, left, right|
        left.val <= right.val ? True.new : False.new
      end

      @scope["if"] = Function.new(["cond", "t", "f"]) do |scope, cond, t, f|
        if cond.val == false
          if Function === f
            f.call [], scope
          else
            f
          end
        else
          if Function === t
            t.call [], scope
          else
            t
          end
        end
      end

      @scope["for"] = Function.new(["list", "func"]) do |scope, list, func|
        list.vals.each do |val|
          func.call [val], scope
        end
      end
    end
  end
end
