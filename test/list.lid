require "lib/extra/test.lid"

suite "LIST TESTS",
      ["filter list"      { l = filter [1 2 3] [x] { x == 2 }
                            test (count l) eq 1 } ],
      ["empty list"       { test (count []) eq 0 }],
      ["list with 1 item" { test (count [9]) eq 1 }]
