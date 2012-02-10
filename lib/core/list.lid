# takes a list and a function
# returns a list in which each item i is replaced with the result of f(i)
map = [l f] { if (count l) == 0,
                 l,
                 { [(f (first l))] + (map (rest l) f) } }

# takes a list and a function
# the return list is filtered, each item i based on the result of f(i)
filter = [l f] { if (count l) == 0,
                    l,
                    { if (f (first l)),
                         { [(first l)] + (filter (rest l) f) },
                         { (filter (rest l) f) } } }

# join list values into a string
join = [l s] { if (count l) == 1,
                  { str (first l) },
                  { ((str (first l)) + s) + (join (rest l) s) } }

.. = [f t] { if f == t,
                [t],
                { [f] + ((f + 1) .. t) } }
