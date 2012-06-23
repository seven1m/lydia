require "lib/extra/test.lid"

suite "RECURSION TESTS",
      ["simple"     { f = [x] { if x == 10,
                                   x,
                                   { f (x + 1) } }
                      test (f 1) eq 10 }]
