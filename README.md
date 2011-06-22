# Airball

This is a toy language I am creating using Ruby and the C library [peg/leg](http://piumarta.com/software/peg/).

The language is taking shape as I learn, and **not meant for real work**. For now, it's starting to resemble a functional language.

## Syntax

    # assignment
    x = 1
    s = "string"
    l = ["list" { out "func" } 3 * 5]

    # functions
    # anything wrapped in { and }
    f1 = { 2 * 3 }
    # function parameters go in [ and ]
    f2 = [x y] { x * y }

    # if/else is just a higher-order function
    if x == 2,
       { out "x is two" },
       { out "x is something else" }

    # recursion
    ^ = [a b] { if b == 1,
                   a,
                   { a * (a ^ (b - 1)) } }

    # calling a function
    f2 2 3          # 6
    (f2 2 3) + (f1) # 12
    2 ^ 3           # 8

    # see example.ball for other available functions

There's a few things to note:

* Functions are anonymous -- you have to explicitly save them to a variable.
* Identifier (variable) names can contain just about any symbol, except equals and some others.
* When a function is defined that starts with a symbol, it is treated as an "infix" function, meaning it is called by placing it in the middle of two arguments.
* `if` is not a special language structure -- it's a function that takes three arguments:
  * a condition
  * a function to execute or a value to return if the condition is true
  * a function to execute or a value to return if the condition is false
* The `if` function can do "elsif" parts if you use an array as the first (and only) arg:

        if [x == 1 { out "x is one" }
            x == 2 { out "x is two" }
                   { out "x is something else" }]

* Function calls are recognized like so:
  * `foo 1` -- one or more *things* separated by space
  * `foo 1 2` -- whitespace between args is all you need
  * `foo 1, 2` -- you can use a comma if you want to
  * `(foo)` -- an identifier wrapped in parentheses (the only way to call a function with no args)
  * `1 * 2` -- actually a function call to `*` with arguments `1` and `2`.
* You must use a comma to line wrap a function call:

        foo "arg one",
            "arg two"

## The Good

* Variables have lexical (static) scope.
* Functions are first-class citizens, and can be passed to and returned from other functions.
* Functions are also closures.
* Airball has a decent test suite.

## The Bad

* Slow, since it runs interpreted on top of Ruby
* No tail-call optimization
* Lots of missing stuff

## Installation

Download the source and build:

    rake build

## Usage

    bin/airball scriptname [args]

## Tests

    rake test

## License

MIT license. Copyright 2011, me.

## Author

Tim Morgan - tim@timmorgan.org
