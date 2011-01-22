# Airball

This is a toy language I am creating using [Parslet](http://kschiess.github.com/parslet/), a nifty Ruby library for building Parsing Expression Grammars (PEGs).

I knew nothing about grammars, parsers, or the like before I started this project, and am learning as I go. **Again, this is a toy language and not meant for real work.**

## Examples

The syntax resembles my favorite language Ruby, but is intended to be functional instead of object-oriented:

    x = 1                 # assignment
    foo = { 2 * 3 }       # function creation
    bar = [x y] { x / y } # function with args
    z = bar 10 5          # function call

Notice the lack of commas and parentheses. If you need to bust some ambiguity, you can wrap an expression in parenthesis, sort of in the Lisp-style:

    x = (5 + 5) / 2
    y = (foo 10) * 5

It isn't obvious here, but any identifiers can have almost any symbol in their name, which means you can define your own `!` function:

    ! = [a b] { ... }

When a function is defined that's all symbols, it is treated as an "infix" function, meaning it is called by placing it in the middle of two arguments.

## The Good

I intend for the language to be a purely functional programming language with as little syntax as possible. Right now, Airball has the following:

* Integers
* Strings (TO DO)
* Arrays (TO DO)
* Functions as first-class citizens
* Functions are also closures
* Command line execution with `bin/airball`
* Command line args
* Ability to print to stdout
* A decent test suite

## The Bad

* Literally about 10 times slower than Ruby
* Probably uses A LOT of memory for a tiny program
* Not finished yet
* It's likely I will lose interest and never finish the thing.

## More Good

I'm learning a lot about parsers. And that's the whole idea here.

## Installation

I may turn this into a gem someday. For now, you'll have to download the source and execute:

    bin/airball scriptname [args]

## Tests

The entire test suite can be run with:

    rake

## License

I hereby release this under the MIT license. Copyright 2011, me.

## Author

Tim Morgan - tim@timmorgan.org
