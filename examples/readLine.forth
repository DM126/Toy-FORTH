//Test

'\n' DEFINE 10 END //newline char ascii code
'\0' DEFINE 0 END //null terminating char

."test.txt" OPEN
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

."Halfway there!" . CR

0 i !
DO
	line i @ #@ char !
	char @ '\0' != IFTHEN
		char @ EMIT	//print each char from the line
	ELSE
		CR
	ENDIF
	i @ 1 + i !
char @ '\0' == UNTIL	//print until null terminating char

."Done!" . CR
