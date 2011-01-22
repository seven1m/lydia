require 'parslet'

module Airball
  class Parser < Parslet::Parser

    SYMBOL_CHARS = "~`!\?@\\#\\$%\\^&\\*\\-_\\+|/,.<>"

    rule(:space)      { match(' ').repeat(1) }
    rule(:space?)     { space.maybe }

    rule(:newline)    { str("\n") }
    rule(:newline?)   { newline.maybe }

    rule(:ns?)        { (space | newline).repeat }

    rule(:integer) do
      match('[0-9]').repeat(1).as(:integer)
    end

    rule(:identifier) do
      match("[a-z]") >> match("[a-z0-9]").repeat
    end

    rule(:symbol) do
      match("[#{SYMBOL_CHARS}]").repeat(1) |
      match("[#{SYMBOL_CHARS}=]").repeat(2)
    end

    rule(:assign) do
      (
        identifier.as(:name) >> space >>
        str("=") >> space >>
        (icall | expr).as(:val) >>
        space? >> newline?
      ).as(:assign)
    end

    rule(:operand) do
      atom |
      str("(") >> (op | icall) >> str(")")
    end

    rule(:op) do
      (
        operand.as(:left) >> space >> symbol.as(:symbol) >> space >> operand.as(:right)
      ).as(:op)
    end

    # line expression
    rule(:lexpr) do
      space? >> newline |
      space? >> (
        icall  |
        assign |
        expr
      ) >> newline?
    end

    # inner expression
    rule(:expr) do
      op              |
      ecall           |
      func.as(:func)  |
      atom
    end

    # simple types, var, int, str, etc.
    rule(:atom) do
      identifier.as(:var)  |
      integer
    end

    # implicit call
    rule(:icall) do
      (
        identifier.as(:name) >> (space >> expr).repeat(1).as(:args)
      ).as(:call)
    end

    # explicit call (with parens)
    rule(:ecall) do
      (
        str("(") >> identifier.as(:name) >> (space >> expr).repeat.as(:args) >> str(")")
      ).as(:call)
    end

    rule(:func) do
      (
        str("[") >> space? >>
        (
          identifier.as(:arg).repeat(1, 1) >>
          (space >> identifier.as(:arg)).repeat
        ).as(:args) >> space? >>
        str("]") >> space?
      ).maybe >>
      str("{") >> ns? >>
      body.as(:body) >>
      ns? >> str("}")
    end

    rule(:body) do
      lexpr.repeat(1)
    end

    root :body
  end
end
