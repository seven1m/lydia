require 'parslet'

module Airball
  class Parser < Parslet::Parser

    SYMBOL_CHARS = "~`!\?@\\#\\$%\\^&\\*\\-_\\+|/,.<>"

    rule(:space)      { match(' ').repeat(1) }
    rule(:space?)     { space.maybe }

    rule(:newline)    { str("\n") }
    rule(:newline?)   { newline.maybe }

    rule(:ns?)        { (space | newline).repeat }

    rule(:cn)         { str(",") >> space? >> newline? }
    rule(:cn?)        { cn.maybe }

    rule(:integer) do
      match['0-9'].repeat(1).as(:integer)
    end

    # strings

    rule(:dquote)    { str('"') }
    rule(:squote)    { str("'") }
    rule(:nondquote) { str('"').absnt? >> any }
    rule(:nonsquote) { str("'").absnt? >> any }
    rule(:escape)    { str("\\") >> any }

    rule(:string) do
      dquote >> (escape | nondquote).repeat.as(:string) >> dquote |
      squote >> (escape | nonsquote).repeat.as(:string) >> squote
    end

    rule(:list) do
      str("[") >> space? >>
      (expr >> space?).repeat.as(:list) >>
      str("]")
    end

    rule(:identifier) do
      match["a-z"] >> match["a-z0-9"].repeat
    end

    rule(:symbol) do
      match[SYMBOL_CHARS].repeat(1) |
      match["#{SYMBOL_CHARS}="].repeat(2)
    end

    rule(:assign) do
      (
        (identifier | symbol).as(:name) >> space >>
        str("=") >> space >>
        (icall | expr).as(:val) >>
        space? >> newline?
      ).as(:assign)
    end

    rule(:operand) do
      list |
      atom |
      str("(") >> (op | icall) >> str(")")
    end

    rule(:op) do
      (
        operand.as(:left) >> space >> symbol.as(:symbol) >> space >> operand.as(:right)
      ).as(:op)
    end

    rule(:comment) do
      str("#") >> match["^\\n"].repeat >> newline?
    end

    # line expression
    rule(:lexpr) do
      space? >> newline |
      comment           |
      space? >> (
        icall           |
        assign          |
        expr
      ) >> newline?
    end

    # inner expression
    rule(:expr) do
      op              |
      ecall           |
      func.as(:func)  |
      list            |
      atom
    end

    # simple types, var, int, str, etc.
    rule(:atom) do
      identifier.as(:var)  |
      string               |
      integer
    end

    # implicit call
    rule(:icall) do
      (
        identifier.as(:name) >> (cn? >> space >> expr).repeat(1).as(:args)
      ).as(:call)
    end

    # explicit call (with parens)
    rule(:ecall) do
      (
        str("(") >> identifier.as(:name) >> (cn? >> space >> expr).repeat.as(:args) >> str(")")
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
