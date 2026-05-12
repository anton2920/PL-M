	NAME main

	ASSUME CS:S, DS:S

S	SEGMENT
	ORG	100H
main	PROC	NEAR
	MOV	AH, 09H
	MOV	DX, OFFSET HelloWorld
	INT	21H

	MOV	AX, 4C00H
	INT	21H
	; NORETURN
main	ENDP

HelloWorld	DB	'Hello, world!', 13, 10, '$'
S	ENDS
	END	main