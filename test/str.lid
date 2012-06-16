require "lib/extra/test.lid"

s1a = "1
2"
s1b = "1\n2"

s2a = "1	2"
s2b = "1\t2"

suite "STRING TESTS",
      ["newline" { test s1a eq s1b }],
      ["tab"     { test s2a eq s2b }]
