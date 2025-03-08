
0 a SET
DO
	0 j SET

	."Test do inside ifthen:" . CR
	1 DUP 0 SWAP ROT == IFTHEN		//if (1 == 1)
		DROP	//drop 0
		DO
			j @ 1 + j !    // j++
			j @ . CR       // print j
		j @ 10 >= UNTIL  // until j >= 10
	ELSE
		."stack operations are broken" . CR
	ENDIF

	CR ."Test nested ifthen inside do:" . CR
	1 i SET
	DO
		i @ .									//print i
		i @ 2 % 0 == IFTHEN						//if (i % 2 == 0)
			.": even" . CR
		ELSE
			.": odd" .
			i @ 3 % 0 == IFTHEN					//if (i % 3 == 0)
				.", divisible by three" . CR
			ELSE
				.", not divisible by three" . CR
			ENDIF
		ENDIF
		i @ 1 + i !			//i++
	i @ 10 > UNTIL			//until i > 10

	CR

	."test logic operators:" . CR
	1 0 NOT AND IFTHEN				//if (1 && !0)
		1 1 NOT OR IFTHEN				//if (1 || !1)
			."both tests passed" . CR
		ELSE
			."test 2 failed" . CR
		ENDIF
	ELSE
		."test 1 failed" . CR
	ENDIF

	CR ."test comparison operations:" . CR
	5 1 < IFTHEN				//if (5 < 1)
		."< is broken" . CR
	ELSE
		."< works" . CR
		3 3 <= 3 4 <= AND IFTHEN	//if (3 <= 3 && 3 <= 4)
			."<= works" . CR
			1 1 == IFTHEN				//if (1 == 1)
				."== works" . CR
				1 1 != IFTHEN				//if (1 != 1)
					."!= is broken" . CR
				ELSE
					."!= works" . CR
					0 0 >= 1 0 >= AND IFTHEN	//if (0 >= 0 && 1 >= 0)
						.">= works" . CR
						5 4 > IFTHEN				//if (5 > 4)
							."> works" . CR
							."all tests passed" . CR
						ELSE
							."> is broken" . CR
						ENDIF
					ELSE
						.">= is broken" . CR
					ENDIF
				ENDIF
			ELSE
				."== is broken" . CR
			ENDIF
		ELSE
			."<= is broken" . CR
		ENDIF
	ENDIF

	CR
	a @ 1 + a !    //a++
a @ 3 >= UNTIL     //until a >= 3
