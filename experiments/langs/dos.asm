	NAME	DOSLIBS

        CGROUP  GROUP   CODE
        DGROUP  GROUP   STACK

	ASSUME	CS:CGROUP, DS:DGROUP, SS:DGROUP

STACK	SEGMENT	STACK	'STACK'
	DW	1024 DUP(0)	; make room for stack
STACK	ENDS

CODE	SEGMENT	PUBLIC	'CODE'
	PUBLIC	dos_std_string_out
	PUBLIC	dos_exit

dos_std_string_out	PROC	NEAR
	PUSH	BP
	MOV	BP, SP

	MOV	AH, 09H
	MOV	DX, [BP+4]
	INT	21H

	MOV	SP, BP
	POP	BP
	RET	2
dos_std_string_out	ENDP

dos_exit	PROC	NEAR
	PUSH	BP
	MOV	BP, SP

	MOV	AH, 4CH
	MOV	AL, [BP+4]
	INT	21H
	; NORETURN
dos_exit	ENDP
CODE	ENDS

	END