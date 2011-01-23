module Airball
  module Errors
    class FunctionNotFound < RuntimeError; end
    class TypeMismatch     < RuntimeError; end
  end
end
