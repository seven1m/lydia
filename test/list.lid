require "lib/extra/test.lid"

suite ["empty list"       { test (count []) eq 0 }],
      ["list with 1 item" { test (count [9]) eq 1 }]
