require 'parslet'

module Airball
  class Parser < Parslet::Parser

    SYMBOL_CHARS = "~`!\?@\\#\\$%\\^&\\*\\-_\\+|/,."

    rule(:space)      { match(' ').repeat(1) }
    rule(:space?)     { space.maybe }

    rule(:newline)    { str("\n") }
    rule(:newline?)   { newline.maybe }

    rule(:ns?)        { (space | newline).repeat }

    rule(:integer) do
      match('[0-9]').repeat(1)
    end

    rule(:identifier) do
      match("[a-z]") >> match("[a-z0-9]").repeat
    end

    rule(:symbol) do
      match("[#{SYMBOL_CHARS}]").repeat(1)
    end

    rule(:assign) do
      identifier.as(:name) >> space >>
      str("=") >> space >>
      expr.as(:val) >>
      space? >> newline?
    end

    rule(:op) do
      atom.as(:left) >> space >> symbol.as(:symbol) >> space >> atom.as(:right)
    end

    rule(:expr) do
      op.as(:op) >> newline? |
      call.as(:call) >> newline? |
      atom >> newline?
    end

    rule(:call) do
      identifier.as(:name) >>
      (space >> (call_with_parens.as(:call) |
                 op.as(:op) |
                 atom)
      ).repeat(1).as(:args) |
      call_with_parens
    end

    rule(:call_with_parens) do
      str("(") >> (call | op | identifier) >> str(")")
    end

    rule(:func) do
      (
        str("[") >> space? >>
        (
          identifier.as(:arg) >>
          (space >> identifier.as(:arg)).repeat
        ).as(:args) >> space? >>
        str("]") >> space?
      ).maybe >>
      str("{") >> ns? >>
      body.as(:body) >>
      ns? >> str("}")
    end

    rule(:atom) do
      identifier.as(:var) | integer.as(:integer) | func.as(:func)
    end

    rule(:body) do
      (assign.as(:assign) | expr).repeat(1) # TODO move assign into expr
    end

    root :body
  end
end
