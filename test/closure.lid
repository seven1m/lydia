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
      ["change var in func" { x = 3
                              f = [x] { x = 4 }
                              (f x)
                              test x eq 4 }],
      ["args shadow vars"   { x = 5
                              f = [x] { x }
                              test (f 6) eq 6 }],
      ["shadow local"       { k = nil
                              f = [s f] { k = s
                                          (f)
                                          k = s }
                              f 1 { s = 2 }
                              test k eq 1 } ],
      ["evaling"            { f1 = [x f2] { f3 = { f2 x }
                                            (f3) }
                              test (f1 12 [y] { y }) eq 12 }]
