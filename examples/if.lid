x = 4

# simple if
if x == 4 { out "four" }

# if/else
if x == 4 { out "four" } { out "not four" }

# if/elsif/else
# to do multiple conditions, wrap all the args in a list
if [x == 1 { out "one" }
    x == 2 { out "two" }
    x == 3 { out "three" }
    { out "something else" }]
