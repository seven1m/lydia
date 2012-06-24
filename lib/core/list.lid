# takes a list and a function
# returns a list in which each item i is replaced with the result of (f i)
map = [l f] { if (count l) == 0,
                 l,
                 { [(f (first l))] + (map (rest l) f) } }

# alias of map, can be used as a dumb "for" loop with side-effects
for = map

# takes a list and a function, returns a new list
# only items that evaluate (f i) == something truthy will be returned in the new list
filter = [l f] { if (count l) == 0,
                    l,
                    { if (f (first l)),
                         { [(first l)] + (filter (rest l) f) },
                         { (filter (rest l) f) } } }

# join list values into a string, using `s` as the glue
join = [l s] { if (count l) == 1,
                  { str (first l) },
                  { ((str (first l)) + s) + (join (rest l) s) } }

# given f (from) and t (to), return a list of consecutive numbers in between
.. = [f t] { if f == t,
                [t],
                { [f] + ((f + 1) .. t) } }

# takes a list and two indices, returns sub-list of values in the range
slice = [l s e] { map (s .. e) [i] { l -> i } }
