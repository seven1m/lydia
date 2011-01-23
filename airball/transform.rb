require 'parslet'

class Object
  def self.iattr(*names)
    class_eval("attr_accessor #{names.map { |n| ':'+n.to_s }.join(', ')}")
    class_eval("def initialize(#{names.join(', ')}) #{names.map { |n| '@'+n.to_s+'='+n.to_s }.join('; ')}; end")
  end
end

module Airball
  class Transform < Parslet::Transform
    rule :integer => simple(:int) do
      Int.new(int.to_i)
    end

    rule :string => simple(:str) do
      Str.new(str.gsub(/\\/, ''))
    end

    rule :call => {:name => simple(:name), :args => sequence(:args)} do
      Call.new(name, args)
    end

    rule :call => {:name => simple(:name)} do
      Call.new(name, [])
    end

    rule :op => {:left => subtree(:left), :symbol => simple(:op), :right => subtree(:right)} do
      Call.new(op, [left, right])
    end

    rule :assign => {:name => simple(:name), :val => subtree(:val)} do
      Assign.new(name, val)
    end

    rule :arg => simple(:arg) do
      arg
    end

    rule :func => {:args => sequence(:args), :body => subtree(:body)} do
      Function.new(args, body)
    end

    rule :func => {:body => subtree(:body)} do
      Function.new([], body)
    end

    rule :var => simple(:name) do
      Var.new(name)
    end
  end
end
