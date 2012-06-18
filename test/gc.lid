require "lib/extra/test.lid"

x = "overwrite me"
x = "new val"

suite "GC TESTS",
      ["overwrite", { h = (heap-)
                      test (filter h [i] { i == "overwrite me" }) eq [] }]
