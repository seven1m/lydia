# assignment
x = 10

# print to stdout
out 8 * 2
out x

# functions
foo = { 2 * 3 }
bar = [x y] { z = x * x
              z + y }

# calling functions
out (foo)
out (bar 3 9)
out 9

# recursive functions: factorial
fact = [x] { if x == 1,
                x,
                { x * (fact x - 1) } }
out (fact 20)

# recursive functions: fibonacci sequence
fib = [n] { if n < 2,
               n,
               { (fib n - 1) + (fib n - 2) } }

# map over a list
fibs = map [1 2 3 4 5 6 7 8 9 10 11 12],
           [n] { fib n }
out (join fibs ", ")

# iterative functions: fibonacci sequence
fib = [x] { curr = 0
            succ = 1
            for (1 .. x),
                [n] { s = curr + succ
                      curr = succ
                      succ = s }
            curr }

# map over a list
fibs = map [1 2 3 4 5 6 7 8 9 10 11 12],
           [n] { fib n }
out (join fibs ", ")

# filter out a list
fibs-over-5 = filter fibs [n] { n > 5 }
out (join fibs-over-5 ", ")

# for loop
list = [{ out "a" } { out "b" } { out "c" }]
for list [x] { (x) }

# while loop
n = 10
while { n > 0 },
      { out n
        n = n - 1 }

# if/elsif/else
if [x == 1 { out "one" }
    x == 2 { out "two" }
    x == 3 { out "three" }
    { out "something else" }]

# script arguments
out "this script: " -script
out "this file: " -filename
out (join ["args:"] + -args, " ")
