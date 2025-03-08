//Test

'\n' DEFINE 10 END //newline char ascii code
'\0' DEFINE 0 END //null terminating char
EOF DEFINE -1 END //End of file

."examples/test.forth" OPEN
0 c SET
DO
	READ c !
	c @ EOF != IFTHEN
		c @ EMIT
	ELSE
		CR
	ENDIF
c @ EOF == UNTIL
