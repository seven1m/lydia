passed-test-count = 0
failed-test-count = 0

test = [v1 f v2] { f v1 v2 }

eq = [a b] { if a == b,
                true,
                { str a " != " b } }

ne = [a b] { if a != b,
                true,
                { str a " == " b } }

suite = { out "\n" (first args)
          tests = (rest args)
          passed = filter tests [t] { n = t -> 0
                                      f = t -> 1
                                      r = (f)
                                      if r == true,
                                         { out "- " n " (passed)" },
                                         { out "- " n " (FAILED)\n  " r
                                           false }}
          passed-test-count = passed-test-count + (count passed)
          failed-test-count = failed-test-count + ((count tests) - (count passed))
          out (count passed) " test(s) passed, ",
              (count tests) - (count passed) " test(s) failed" }
