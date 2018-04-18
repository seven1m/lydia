# Lydia

Lydia is an interpreted functional language that's taking shape as I learn, and **not meant for real work**.

The syntax is very terse, as you can see below:

## Syntax

    # assignment
    x = 1
    s = "string"
    l = ["list" { out "func" } 3 * 5]

    # functions
    # anything wrapped in { and }, optionally having arguments in [ and ]
    f1 = { 2 * 3 }
    f2 = [x y] { x * y }

    # if/else is just a higher-order function
    if x == 2,
       { out "x is two" },
       { out "x is something else" }

    # recursion & infix calling
    ^ = [a b] { if b == 1,
                   a,
                   { a * (a ^ (b - 1)) } }

    # calling a function
    f2 2 3          # 6
    (f2 2 3) + (f1) # 12
    2 ^ 3           # 8

    # see the examples directory for more

There's a few things to note:

* A Function is anything between `{` and `}` and is only named if you save it to a variable, e.g. `foo = { bar }`
* Identifier (variable) names can contain just about any symbol, except equals and some others.
* Giving a function a name that contains only symbols makes it work as an "infix" function, meaning it is called by placing it in the middle of two arguments.
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
* Use a comma to line wrap arguments to a function call:

        foo "arg one",
            "arg two"

## The Good

* Simple, flexible syntax
* Variables have lexical (static) scope.
* Functions are first-class citizens, and can be passed to and returned from other functions.
* Functions are closures.
* Tail-call optimization.

## The Bad

Things that are missing (for now):

* Lots of missing functions.
* Associative array is missing.
* Virtually *no* standard library.

## Installation

Download the source and build:

    sudo apt install libatomic-ops-dev libgmp3-dev
    make build

The build task downloads and builds libgc -- if that fails, you may need to build libgc manually (see the Makefile) and move the shared libs into `ext/lib` and the headers into `ext/include`.

## Usage

    bin/lydia scriptname [args]

## Tests

    make test

## License

Copyright (c) Tim Morgan. Licensed MIT.

## Author

Tim Morgan: [timmorgan.org](http://timmorgan.org) | tim@timmorgan.org | [@seven1m](http://twitter.com/seven1m)
