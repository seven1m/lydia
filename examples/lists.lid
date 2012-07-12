fibs = [1 1 2 3 5 8 13 21 34 55 89 144]

# filter out a list
fibs-over-5 = filter fibs [n] { n > 5 }
out fibs-over-5
