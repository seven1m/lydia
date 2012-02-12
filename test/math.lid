require "lib/extra/test.lid"

suite "MATH TESTS",
      ["num +=" { a = 1
                  a += 1
                  test a eq 2 }],
      ["num -=" { a = 1
                  a -= 1
                  test a eq 0 }],
      ["num *=" { a = 2
                  a *= 3
                  test a eq 6 }],
      ["num /=" { a = 6
                  a /= 2
                  test a eq 3 }]
