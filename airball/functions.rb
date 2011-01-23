module Airball
  module Functions
    def define_function(name, args, &body)
      @scope[name.to_s] = Function.new(args.map(&:to_s), &body)
    end

    def build_functions
      define_function :out, [:expr] do |scope, expr|
        scope["stdout"].write expr.to_s + "\n"
      end

      define_function :+, [:left, :right] do |scope, left, right|
        if Airball::Int === left
          Int.new(left.val + right.val)
        elsif Airball::List === left
          List.new(left.vals + right.vals)
        else
          raise TypeMismatch, "Don't know how to add #{left} and #{right}"
        end
      end

      define_function :-, [:left, :right] do |scope, left, right|
        Int.new(left.val - right.val)
      end

      define_function :*, [:left, :right] do |scope, left, right|
        Int.new(left.val * right.val)
      end

      define_function :/, [:left, :right] do |scope, left, right|
        Int.new(left.val / right.val)
      end

      @scope["true"] = True.new
      @scope["false"] = False.new

      define_function :==, [:left, :right] do |scope, left, right|
        left.val == right.val ? True.new : False.new
      end

      define_function :>, [:left, :right] do |scope, left, right|
        left.val > right.val ? True.new : False.new
      end

      define_function :>=, [:left, :right] do |scope, left, right|
        left.val >= right.val ? True.new : False.new
      end

      define_function :<, [:left, :right] do |scope, left, right|
        left.val < right.val ? True.new : False.new
      end

      define_function :<=, [:left, :right] do |scope, left, right|
        left.val <= right.val ? True.new : False.new
      end

      define_function :if, [:cond, :t, :f] do |scope, cond, t, f|
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

      define_function :for, [:list, :func] do |scope, list, func|
        list.vals.each do |val|
          func.call [val], scope
        end
      end

      define_function :first, [:list] do |scope, list|
        list.vals.first
      end

      define_function :rest, [:list] do |scope, list|
        List.new(list.vals[1..-1])
      end

      define_function :count, [:list] do |scope, list|
        Int.new(list.vals.length)
      end
    end
  end
end
