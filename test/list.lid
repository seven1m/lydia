require "lib/extra/test.lid"

suite "LIST TESTS",
      ["count list"       { test (count [9]) eq 1 }],
      ["count empty list" { test (count []) eq 0 }],
      ["filter list"      { l = filter [1 2 3] [x] { x == 2 }
                            test (count l) eq 1 } ],
      ["map list"         { l = map [1 2 3] [x] { x * 2 }
                            test (l) eq [2 4 6] } ],
      ["empty compare"    { test [] eq [] }],
      ["compare =="       { test [1] eq [1] }],
      ["compare !="       { test [2] ne [1] }]
