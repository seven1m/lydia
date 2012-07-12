# recursive version of the factorial function
# NOT tail-call optimized

fact = [x] { if x == 1,
                x,
                { x * (fact x - 1) } }
out (fact 20)
