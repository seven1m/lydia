# Memory Management in Lidija

    MEM       |01|02|03|04|05|06|
              | t| h| i| n| g| s| <- data
              | 2| 2| 2| 1| 1| 1| <- ref counts

    HEAP      |11|22|33|44|55|66|
              |01|02|03|04|05|06| <- pointers

    CLOSURE   | a| b| c|          <- named vars
    |         |11|22|33|          <- pointers to pointers
    |
    |-CLOSURE | a| b| c| x| y| z|
              |11|22|33|44|55|66|

    **ref => *value => [mem]

## Getting a variable

    => CLOSURE.get("a") = 11
    == deref *11        = 01
    <= return 01        = "t"

## Setting a variable

    == 06                   = "s"
    => CLOSURE.set("a", 06)
    == HEAP.set(11, 06)     = 01

## Garbage Collection

When a variable is added to a closure, its ref count
is incremented. When a closure is cloned,
all its variables' ref counts are incremented.

When a closure is freed, each of its variables'
ref count is decremented.

Then GC is run on the HEAP, which iterates each value
and, if its ref count is zero, frees it.
