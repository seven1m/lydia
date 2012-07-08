require "lib/extra/test.lid"

suite "LIST TESTS",
      ["count"            { test (count [9]) eq 1 }],
      ["count empty"      { test (count []) eq 0 }],
      ["filter"           { l = filter [1 2 3] [x] { x == 2 }
                            test l eq [2] } ],
      ["map"              { l = map [1 2 3] [x] { x * 2 }
                            test l eq [2 4 6] } ],
      ["slice"            { l = slice [1 2 3 4] 1 2
                            test l eq [2 3] } ],
      ["empty compare"    { test [] eq [] }],
      ["compare =="       { test [1] eq [1] }],
      ["compare !="       { test [2] ne [1] }],
      ["first of empty"   { test (first []) eq nil }],
      ["index 0 of empty" { test ([] -> 0) eq nil }],
      ["index past end"   { test ([1] -> 1) eq nil }],
      ["rest"             { test (rest [1 2 3]) eq [2 3] }],
      ["rest of single"   { test (rest [1]) eq [] }],
      ["rest of empty"    { test (rest []) eq [] }]
