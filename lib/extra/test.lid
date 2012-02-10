test = [v1 f v2] { f v1 v2 }

eq = [a b] { if a == b,
                true,
                { out a " does not eq " b } }

suite = { passed = map args [t] { out "- " t -> 0
                                  (t -> 1) == true }
          out ""
          out (count passed) " test(s) passed, ",
              (count args) - (count passed) " test(s) failed" }

