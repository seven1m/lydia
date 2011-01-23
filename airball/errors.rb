module Airball
  module Errors
    class FunctionNotFound      < RuntimeError; end
    class TypeMismatch          < RuntimeError; end
    class ArgumentCountMismatch < RuntimeError; end
  end
end
