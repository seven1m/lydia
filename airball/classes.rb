module Airball
  class Scope
    def initialize(parent=nil)
      @parent = parent
      @vars = {}
    end

    def [](name)
      @vars[name] || @parent && @parent[name]
    end

    def []=(name, value)
      @vars[name] = value
    end
  end

  class Obj
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

  class Function < Obj
    attr_accessor :args

    def initialize(args, body=nil, &body_proc)
      @args = args
      @body = body
      @body_proc = body_proc
    end

    # TODO functions are not yet closures
    def call(args, scope)
      scope = Scope.new(scope)
      args.map! { |a| a.eval(scope) }
      @args.each_with_index { |a, i| scope[a] = args[i] }
      if @body_proc
        @body_proc.call(scope, *args)
      else
        @body[0..-2].each do |expr|
          expr.eval(scope)
        end
        @body.last.eval(scope) # return last result from function
      end
    end

    def eval(scope)
      self
    end
  end

  class Call < Obj
    iattr :name, :args
    def eval(scope)
      function = scope[name]
      if Function === function
        function.call(args, scope)
      else
        raise Errors::FunctionNotFound, "'#{name}' could not be found in the current scope."
      end
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
  end
end
