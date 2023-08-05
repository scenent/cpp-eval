# cpp-eval
## preview
```cpp
1 + 2 -> [Int, "3"]
1.0 + 2.0 -> [Float, "3.000000"]
1 + 2 * 3 -> [Int, "7"]
(1 + 2) * 3 -> [Int, "9"]
1 + 2 == 3 -> [True, "true"]
1 + 2 == 3 and 2 * 3 == 6 -> [True, "true"]
0 == 1 or 1 == 1 -> [True, "true"]
1 * 2 == 2 or 0 == 1 and 1 * 3 == 3 or 0 == 1 -> [True, "true"]
1 == 1 and 2 == 2 and 3 != 3 -> [False, "false"]
"Hello" + " " + "World!" -> [String, "Hello World!"]
true and false -> [False, "false"]
-(1 + 2 * 3) -> [Int, "-7"]
not 1 == 1 or not 1 == 2 -> [True, "true"]
-cos(0) + sin(0) + tan(0) -> [Float, "-1.000000"]
sin 1 -> [Float, "0.841471"]
pow((1 * 4), 2) + 4 -> [Float, "20.000000"]
```
