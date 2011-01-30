module Airball
  class Store
    attr_accessor :values

    def initialize
      @values = []
    end

    def <<(value)
      @values << value
      @values.length - 1
    end

    def [](index)
      @values[index]
    end

    def []=(index, value)
      @values[index] = value
    end
  end

  class Closure
    attr_accessor :store, :vars, :locals

    def initialize(parent_or_store)
      if Store === parent_or_store
        @store = parent_or_store
        @vars = {}
        @locals = {}
      else
        @store = parent_or_store.store
        @vars = parent_or_store.vars.dup
        @locals = parent_or_store.locals.dup
      end
    end

    def get(name, skip_locals=false)
      if !skip_locals and @locals.has_key?(name)
        @locals[name]
      elsif @vars.has_key?(name)
        @store[@vars[name]]
      else
        raise Errors::VariableNotFound, "var '#{name}' cannot be found"
      end
    end

    def [](name)
      get(name)
    end

    def set(name, value, local=false)
      if local or @locals.has_key?(name)
        @locals[name] = value
      elsif @vars.has_key?(name)
        @store[@vars[name]] = value
      else
        @vars[name] = @store << value
      end
    end

    def []=(name, value)
      set(name, value)
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

    def call(name, args, calling_scope)
      # args come from calling scope
      args = args.map { |a| a.eval(calling_scope) }
      # create a disposable scope for evaluating the body of the function
      scope = Closure.new(@closure)
      # allow self-calling (recursion)
      scope.set(name, self, :local) if name
      # function body scope is based on the closing (lexical) scope of the function
      # but args have to be local only (they cannot change the value of an enclosing var)
      @args.each_with_index { |a, i| scope.set(a, args[i], :local) }
      if @body_proc
        @body_proc.call(scope, *args)
      else
        @body[0..-2].each do |expr|
          expr.eval(scope)
        end
        @body.last.eval(scope) if @body.last # return last result from function
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
