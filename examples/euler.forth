//Project Euler implementations in forth

// x 1 isDivisible()
isDivisible() DEFINE
    SWAP @ SWAP % 0 ==
END

// x ++
++ DEFINE
    DUP @ 1 + SWAP !
END

// 1 endl
endl DEFINE
    . CR
END

add() DEFINE
    @ SWAP @ +
END

// solution results problemNumber printResults()
printResults() DEFINE
    ."Problem " . . .":" endl
    ."Sum:      " . @ endl
    ."Solution: " . endl CR
END

0 sum SET
1 i SET

// Problem 1 ~~~~~~~~~~~~~~~~~~~~~
DO
    i 3 isDivisible() i 5 isDivisible() OR IFTHEN
        i sum add() sum !
    ENDIF
    i ++
i @ 1000 == UNTIL
233168 sum 1 printResults()




// Problem 2 ~~~~~~~~~~~~~~~~~~~~~
1 i !
2 j SET
0 temp SET
2 sum !
4000000 max SET

DO
    j @ temp !
    i j add() j !
    temp @ i !
    j @ max @ < j 2 isDivisible() AND IFTHEN
        j sum add() sum !
    ENDIF
j @ max @ >= UNTIL
4613732 sum 2 printResults()




// Problem 3 ~~~~~~~~~~~~~~~~~~~~~
//600851475143 . CR //too large
