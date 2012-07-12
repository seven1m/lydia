# recursive version of the fibonacci sequence
# NOT tail-call optimized

fib = [n] { if n < 2,
               n,
               { (fib n - 1) + (fib n - 2) } }

fibs = map [1 2 3 4 5 6 7 8 9 10 11 12] fib

out (join fibs ", ")
