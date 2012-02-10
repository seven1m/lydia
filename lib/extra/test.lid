test = [v1 f v2] { f v1 v2 }

eq = [a b] { if a == b,
                true,
                { str "  " a " does not equal " b } }

ne = [a b] { if a != b,
                true,
                { str "  " a " is equal to " b } }

suite = { out (first args)
          tests = (rest args)
          passed = filter tests [t] { n = t -> 0
                                      f = t -> 1
                                      r = (f)
                                      if r == true,
                                         { out "- " n " (passed)" },
                                         { out "- " n " (FAILED)\n  " r
                                           false }}
          out (count passed) " test(s) passed, ",
              (count tests) - (count passed) " test(s) failed" }
          out ""
