# recursive version of the fibonacci sequence
# tail-call optimized

fib = [x] { f = [i c n] { if i == 0,
                             c,
                             { f (i - 1) n (c + n) } }
            f x 0 1 }


fibs = map [1 2 3 4 5 6 7 8 9 10 11 12] fib

out (join fibs ", ")
