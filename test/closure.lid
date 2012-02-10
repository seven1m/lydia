require "lib/extra/test.lid"

x = 1
f = { x }
x = 2
y = (f)

suite "CLOSURE TESTS",
      ["top level change"   { test y eq 2 }],
      ["nested, change var" { x = 3
                              f = { x }
                              x = 4
                              test (f) eq 4 }],
      ["args shadow vars"   { x = 5
                              f = [x] { x }
                              test (f 6) eq 6 }]
