module Airball
  module Functions
    def define_function(name, args, &body)
      @scope[name.to_s] = Function.new(args.map(&:to_s), &body)
      @scope[name.to_s].closure = @scope
    end

    def build_functions
      define_function :out, [:expr] do |scope, expr|
        scope["stdout"].write expr.to_s + "\n"
      end

      define_function :+, [:left, :right] do |scope, left, right|
        if Airball::Int === left && Airball::Int === right
          Int.new(left.val + right.val)
        elsif Airball::Str === left && Airball::Str === right
          Str.new(left.val + right.val)
        elsif Airball::List === left && Airball::List === right
          List.new(left.vals + right.vals)
        else
          raise Errors::TypeMismatch, "Don't know how to add #{left} and #{right}"
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

      define_function '&&', [:left, :right] do |scope, left, right|
        left.val == false || right.val == false ? False.new : True.new
      end

      define_function '||', [:left, :right] do |scope, left, right|
        left.val == false && right.val == false ? False.new : True.new
      end

      define_function :if, [:cond, :t, :f] do |scope, cond, t, f|
        if List === cond
          if cond.vals.length >= 3
            result = nil
            cond.vals.each_slice(2) do |part|
              if part.length == 2
                c, f = part
                unless c.val == false
                  if Function === f
                    result = f.call nil, [], scope
                  else
                    result = f
                  end
                  break
                end
              else
                f = part.first
                if Function === f
                  result = f.call nil, [], scope
                else
                  result = f
                end
              end
            end
            result
          else
            raise Errors::ArgumentCountMismatch, "Not enough arguments for 'if'"
          end
        else
          if cond.val == false
            if Function === f
              f.call nil, [], scope
            else
              f
            end
          else
            if Function === t
              t.call nil, [], scope
            else
              t
            end
          end
        end
      end

      define_function :for, [:list, :func] do |scope, list, func|
        list.vals.each do |val|
          func.call nil, [val], scope
        end
      end

      define_function :while, [:cond, :func] do |scope, cond, func|
        while cond.call(nil, [], scope).val != false
          func.call nil, [], scope
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

      define_function :str, [:expr] do |scope, expr|
        Str.new(expr.to_s)
      end
    end
  end
end
