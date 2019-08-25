# Hack Assembly Language Compiler

* What is Hack Assembly?
    * There are some documents on the internet.
    * [Introduction to Hack Assembly Language](http://www.marksmath.com/tecs/hack-asm/hack-asm.html)
    * [Assembly Language Tricks for the HACK computer](http://www.dragonwins.com/domains/getteched/csm/CSCI410/references/hack.htm)


* This is a simple compiler that can compile JLang to Hack Assembly.

* This compiler implement these functions:
    * assembly optimization
        * dead code elimination
        * operation combine
        * duplicate operation elimination
    * stack frame
        * return address
        * function arguments
        * return value
    * loop & conditions
        * with variable life cycle in a block.
    * built-in library
        * output
            * draw rectangle
            * draw number
            * draw chars
            * clear screen
        * input
            * read keys
        * math
            * abs
            * lowbit
            * xor
            * multiply
            * divide
            * mod
            * min
            * max
        * memory
            * allocate
            * free
