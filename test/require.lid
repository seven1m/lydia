require "lib/extra/test.lid"

require-count = 0
require "test/require-ex.lid"
require "test/require-ex.lid"

suite "REQUIRE TESTS",
      ["only once" { test require-count eq 1 }]
