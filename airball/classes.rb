module Airball
  class Scope
    attr_reader :vars

    def initialize(parent=nil)
      @parent = parent
      @vars = {}
    end

    def [](name)
      @vars[name] ||
      raise(Errors::VariableNotFound, "var '#{name}' cannot be found")
    end

    def []=(name, value)
      set(name, value)
    end

    def set(name, value)
      @vars[name] = value
    end

    def names
      if @parent
        hash = @parent.names
      else
        hash = {}
      end
      @vars.keys.each do |name|
        hash[name] = self
      end
      hash
    end
  end

  class Closure
    attr :names, :scope

    # must be the parent scope of the function
    def initialize(scope)
      @names = scope.names
      @scope = Scope.new(scope)
    end

    def [](name)
      if scope = @names[name]
        if Closure === scope
          raise "This scope cannot be a closure."
        end
        scope[name]
      else
        raise Errors::VariableNotFound, "var '#{name}' cannot be found"
      end
    end

    def []=(name, value)
      if scope = @names[name]
        scope[name] = value
      else
        set(name, value)
      end
    end

    def set(name, value)
      @names[name] = @scope
      @scope[name] = value
    end
  end

  class Obj
    def self.airball_name
      name.split('::').last
    end

    def to_s
      "<#{self.class.airball_name}>"
    end
  end

  class Int < Obj
    iattr :val

    def eval(scope)
      self
    end

    def to_s
      val.to_s
    end
  end

  class Str < Obj
    iattr :val

    def eval(scope)
      self
    end

    def to_s
      val
    end
  end

  class List < Obj
    iattr :vals

    def eval(scope)
      List.new(vals.map { |v| v.eval(scope) })
    end

    def to_s
      vals.map { |v| v.to_s }.join("\n")
    end
  end

  class Rng < Obj
    iattr :first, :last

    def eval(scope)
      # TODO really???
      f = first.eval(scope).val
      l = last.eval(scope).val
      nums = (f..l).map { |n| Int.new(n) }
      List.new(nums).eval(scope)
    end
  end

  class True < Obj
    def eval(scope)
      self
    end

    def val
      true
    end

    def to_s
      "true"
    end
  end

  class False < Obj
    def eval(scope)
      self
    end

    def val
      false
    end

    def to_s
      "false"
    end
  end

  class Function < Obj
    attr_accessor :args, :closure

    def initialize(args, body=nil, &body_proc)
      @args      = args
      @body      = body
      @body_proc = body_proc
    end

    def call(name, args, scope)
      # args come from calling scope
      args = args.map { |a| a.eval(scope) }
      # allow self-calling (recursion)
      @closure.names[name] = Closure === scope ? scope.names[name] : scope
      # function body scope is based on the closing (lexical) scope of the function
      @args.each_with_index { |a, i| @closure.set(a, args[i]) }
      if @body_proc
        @body_proc.call(@closure, *args)
      else
        @body[0..-2].each do |expr|
          expr.eval(@closure)
        end
        @body.last.eval(@closure) # return last result from function
      end
    end

    def eval(scope)
      @closure = Closure.new(scope)
      self
    end
  end

  class Call < Obj
    iattr :name, :args

    def eval(scope)
      function = scope[name]
      if Function === function
        if DEBUG and DEBUG.include?(:calls)
          puts "DEBUG: calling '#{name}' with args #{args.inspect}" 
          result = function.call(name, args, scope)
          puts "DEBUG: result from '#{name}' => #{result}"
          result
        else
          function.call(name, args, scope)
        end
      else
        raise Errors::FunctionNotFound, "'#{name}' could not be found in the current scope."
      end
    end

    def to_s
      "<Call #{name}>"
    end
  end

  class Assign < Obj
    iattr :name, :val

    def eval(scope)
      scope[name] = val.eval(scope)
    end
  end

  class Var < Obj
    iattr :name

    def eval(scope)
      scope[name]
    end

    def to_s
      "<Var #{name}>"
    end
  end
end
