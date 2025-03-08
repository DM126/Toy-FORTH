//Forth interpreter written in Forth

//helper variables---------------------
-1 eof SET
0 '\0' SET


//helper functions---------------------
increment() DEFINE
	DUP @ 1 + SWAP !
END

//buffer definition--------------------
255 bufferCapacity SET
0 bufferSize SET
bufferCapacity @ buffer ALLOT

//no args, returns bool
isFull() DEFINE
	bufferSize @ bufferCapacity @ ==
END

//no args, void
expandBuffer() DEFINE
	bufferCapacity @ 2 *
	DUP bufferCapacity !
	buffer ALLOT
END

//Takes char arg, void
addToBuffer() DEFINE
	isFull() IFTHEN
		expandBuffer()
	ENDIF

	buffer bufferSize @ #! 
	bufferSize increment()
END



//****************************************
//                  MAIN
//****************************************

//Place each char of the file into the buffer
."examples/test.forth" OPEN
DO
	READ
	DUP DUP addToBuffer() EMIT
eof @ == UNTIL
'\0' buffer bufferSize @ 1 - #! 

