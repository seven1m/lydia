require "lib/extra/test.lid"

out "testing the test library..."

# capture stdout
# FIXME this should change later once we have real streams
captured = ''
real-out = out
out = { captured = captured + (join args "") }

suite 'TEST TESTS',
      ["passed test" { test 0 eq 0 }],
      ["failed test" { test 1 eq 0 }]

# put everything back
out = real-out

should-be = "\nTEST TESTS- passed test (passed)- failed test (FAILED)\n  1 != 01 test(s) passed, 1 test(s) failed"

if captured == should-be,
   { out "- pass (passed)"
     out "- fail (passed)"
     failed-test-count = failed-test-count - 1
     passed-test-count = passed-test-count + 1
     out (suite-summary 2 0) },
   { out "  broken!\n[" captured "]\n != \n[" should-be "]" }
