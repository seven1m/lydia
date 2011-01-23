module Airball
  module Functions
    def build_functions
      @scope["out"] = Function.new(["expr"]) do |scope, expr|
        @scope["stdout"].write expr.to_s + "\n"
      end

      @scope["+"] = Function.new(["left", "right"]) do |scope, left, right|
        if Airball::Int === left
          Int.new(left.val + right.val)
        elsif Airball::List === left
          List.new(left.vals + right.vals)
        else
          raise TypeMismatch, "Don't know how to add #{left} and #{right}"
        end
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

      @scope["first"] = Function.new(["list"]) do |scope, list|
        list.vals.first
      end

      @scope["rest"] = Function.new(["list"]) do |scope, list|
        List.new(list.vals[1..-1])
      end

      @scope["count"] = Function.new(["list"]) do |scope, list|
        Int.new(list.vals.length)
      end
    end
  end
end
