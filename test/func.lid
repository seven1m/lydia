require "lib/extra/test.lid"

suite "FUNCTION TESTS",
      ["one expr, return num"     { f = { 1 }
                                    test (f) eq 1 }],
      ["multi expr, return num"   { f = { 1
                                          2 }
                                    test (f) eq 2 }],
      ["one expr, return true"    { f = { true }
                                    test (f) eq true }],
      ["one expr, return false"   { f = { false }
                                    test (f) eq false }],
      ["multi expr, return true"  { f = { 1
                                          true }
                                    test (f) eq true }],
      ["multi expr, return false" { f = { 1
                                          false }
                                    test (f) eq false }]
