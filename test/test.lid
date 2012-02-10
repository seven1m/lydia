require "lib/extra/test.lid"

out "testing the test library..."

# capture stdout
# FIXME this should change later once we have real streams
real-out = out
out = str

res = suite nil,
            ["passed test" { test 0 eq 0 }],
            ["failed test" { test 1 eq 0 }]

# put everything back
out = real-out

if res == "1 test(s) passed, 1 test(s) failed",
   { out "  seems OK" },
   { out "  broken!" }
