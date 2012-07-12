# iterative version of the fibonacci sequence

fib = [x] { curr = 0
            succ = 1
            for (1 .. x),
                [n] { s = curr + succ
                      curr = succ
                      succ = s }
            curr }

fibs = map [1 2 3 4 5 6 7 8 9 10 11 12] fib

out (join fibs ", ")
