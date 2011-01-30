module Airball
  module Errors
    class FunctionNotFound      < RuntimeError; end
    class TypeMismatch          < RuntimeError; end
    class ArgumentCountMismatch < RuntimeError; end
    class VariableNotFound      < RuntimeError; end
  end
end
