require 'parslet'

module Airball
  class Parser < Parslet::Parser

    SYMBOL_CHARS = "~`!\?@\\#\\$%\\^&\\*\\-_\\+|/,."

    rule(:space)      { match(' ').repeat(1) }
    rule(:space?)     { space.maybe }

    rule(:newline)    { str("\n") }
    rule(:newline?)   { newline.maybe }

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
      identifier >> space >> str("=") >> space >> expr
    end

    rule(:op) do
      atom.as(:left) >> space >> symbol.as(:symbol) >> space >> atom.as(:right)
    end

    rule(:expr) do
      op.as(:op) >> newline? |
      call.as(:call) >> newline?
    end

    rule(:call) do
      identifier.as(:name) >> (space >> (op.as(:op) | atom)).repeat.as(:args)
    end

    rule(:atom) do
      identifier.as(:var) | integer.as(:integer)
    end

    rule(:body) do
      (assign.as(:assign) | expr).repeat(1)
    end

    root :body
  end
end
