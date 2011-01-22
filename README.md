# Airball

This is a toy language I am creating using [Parslet](http://kschiess.github.com/parslet/), a nifty Ruby library for building Parsing Expression Grammars (PEGs).

I knew nothing about grammars, parsers, or the like before I started this project, and am learning as I go. **Again, this is a toy language and not meant for real work.**

## Syntax

    # assignment
    x = 1

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

There's a few things to understand here:

* Functions are anonymous by default -- you have to explicitly save them to a variable.
* Identifier (variable) names can contain just about any symbol, except equals and some others.
* When a function is defined that starts with a symbol, it is treated as an "infix" function, meaning it is called by placing it in the middle of two arguments.
* `if` is not a special language structure -- it's a function that takes three arguments:
  * a condition
  * a function to execute or a value to return if the condition is true
  * a function to execute or a value to return if the condition is false
* You cannot (yet) pass an "elseif" part to `if`.
* Function calls are recognized like so:
  * `foo 1` -- one or more *things* separated by space
  * `(foo)` -- an identifier wrapped in parentheses (the only way to call a function with no args)
  * `1 * 2` -- actually a function call to "\*" with arguments `1` and `2`.
* Use a comma to wrap a function call to the next line (like a poem):

    foo "arg one",
        "arg two"

## The Good

* Variables are dynamically scoped and immutable.
* Functions as first-class citizens, and can be passed to and returned from other functions.
* Functions are also closures.
* Airball has a decent test suite.

## The Bad

* Literally about 10 times slower than Ruby
* Not finished yet

## Installation

I may turn this into a gem someday. For now, you'll have to download the source and execute:

    bin/airball scriptname [args]

You'll need Ruby 1.9.2+ installed.

## Tests

The entire test suite can be run with:

    rake

## License

I hereby release this under the MIT license. Copyright 2011, me.

## Author

Tim Morgan - tim@timmorgan.org
