# Airball

This is a toy language I am creating using the Ruby library [Parslet](http://kschiess.github.com/parslet/) and the C library [peg/leg](http://piumarta.com/software/peg/). Airball uses Parslet's [Transformation](http://kschiess.github.com/parslet/transform.html) library to convert flat Ruby hashes into Airball class instances.

I knew nothing about grammars, parsers, or the like before I started this project, and am learning as I go. **Again, this is a toy language and not meant for real work.**

The language is taking shape as I learn. For now, it's starting to resemble a **functional language**.

## Syntax

    # assignment
    x = 1
    s = "string"
    l = ["list" { out "func" } 3 * 5]

    # functions
    f1 = { 2 * 3 }
    f2 = [x y] { x * y }

    # if/else
    if x == 2,
       { out "x is two" },
       { out "x is something else" }

    # recursion
    ^ = [a b] { if y == 1,
                   x,
                   { x * (x ^ (y - 1)) } }

    # calling a function
    f2 2 3          # 6
    (f2 2 3) + (f1) # 12
    2 ^ 3           # 8

    # see example.ball for other available functions

There's a few things to understand here:

* Functions are anonymous by default -- you have to explicitly save them to a variable.
* Identifier (variable) names can contain just about any symbol, except equals and some others.
* When a function is defined that starts with a symbol, it is treated as an "infix" function, meaning it is called by placing it in the middle of two arguments.
* `if` is not a special language structure -- it's a function that takes three arguments:
  * a condition
  * a function to execute or a value to return if the condition is true
  * a function to execute or a value to return if the condition is false
* The `if` function can do "elsif" parts if you use an array as the first arg:

        if [x == 1 { out "x is one" }
            x == 2 { out "x is two" }
                   { out "x is something else" }]

* Function calls are recognized like so:
  * `foo 1` -- one or more *things* separated by space
  * `(foo)` -- an identifier wrapped in parentheses (the only way to call a function with no args)
  * `1 * 2` -- actually a function call to `*` with arguments `1` and `2`.
* Use a comma to wrap a function call to the next line (like a poem):

        foo "arg one",
            "arg two"

## The Good

* Variables have lexical (static) scope.
* Functions are first-class citizens, and can be passed to and returned from other functions.
* Functions are also closures.
* Airball has a decent test suite.

## The Bad

* Literally about 10 times slower than Ruby
* Not finished yet

## Installation

I may turn this into a gem someday. For now, you'll have to download the source and build:

    rake build

You'll need Ruby 1.9.2+ and the "parslet" gem installed.

## Usage

    bin/airball scriptname [args]

## Tests

The test suite can be run with:

    rake test

## License

MIT license. Copyright 2011, me.

## Author

Tim Morgan - tim@timmorgan.org
