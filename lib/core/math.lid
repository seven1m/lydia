# exponentiation (x to the power of y)
^ = [x y] { if y == 1,
               x,
               { x * (x ^ (y - 1)) } }

+= = [x y] { x = x + y }
-= = [x y] { x = x - y }
*= = [x y] { x = x * y }
/= = [x y] { x = x / y }
