//5 3 + .
//Helper functions
increment() DEFINE //takes variable, void
	DUP @ 1 + SWAP !
END

decrement() DEFINE //takes variable, void
	DUP @ 1 - SWAP !
END



//Begin stack definition
//----------------------------------------------------------
255 stack.CAPACITY SET
0 stack.size SET //Also points to head+1
stack.CAPACITY @ stack ALLOT

stack.push() //takes int, void
DEFINE
	stack.size @ stack.CAPACITY @ < IFTHEN
		stack stack.size @ #!
		stack.size increment()
	ELSE
		."Stack is at capacity." . CR
	ENDIF
END

stack.pop() //no args, returns integer
DEFINE
	stack.isEmpty() NOT IFTHEN
		stack.peek()
		0 stack stack.size @ 1 - #!
		stack.size decrement()
	ELSE
		."Cannot pop: stack is empty." . CR
	ENDIF
END

stack.peek() //no args, returns integer
DEFINE
	stack.isEmpty() NOT IFTHEN
		stack stack.size @ 1 - #@
	ELSE
		."Stack is empty." . CR
	ENDIF
END

stack.isEmpty() //no args, returns boolean
DEFINE
	stack.size @ 0 ==
END

stack.clear() //no args, void
DEFINE
	DO
		stack.isEmpty() NOT IFTHEN
			stack.size decrement()
		ENDIF
		0 stack stack.size @ #!
	stack.isEmpty() UNTIL
END
//----------------------------------------------------------
//End stack definition



//main:
'\n' DEFINE 10 END //newline char ascii code
'\0' DEFINE 0 END //null terminating char
'_' DEFINE 32 END //space (not underscore)
EOF DEFINE -1 END //end of file

."examples/test.forth" OPEN
255 line ALLOT
0 i SET
0 char SET
DO
	READ char !
	char @ '\n' != IFTHEN
		char @ line i @ #! //Store the char at index i
	ELSE
		'\0' line i @ #! //End with null char
	ENDIF
	i @ 1 + i !
char @ '\n' == UNTIL //Read until newline

."Done!" . CR

