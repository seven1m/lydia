require "lib/extra/test.lid"

x = 1
y = x
z = 1
get-x = { x }
x = x + 1

suite "ASSIGN TESTS",
      ["increment"    { test x eq 2 } ],
      ["change outer" { x = 3
                        test (get-x) eq 3 } ],
      ["deep change"  { f = { z = 2 }
                        (f)
                        test z eq 2 } ],
      ["copy var"     { test y eq 1 } ]
