code segment
	MOV DL, '!'	; PRINT EXCLAMATION MARK
	MOV AH, 02
	INT 21h

	MOV DL, 10 	; PRINT NEW LINE
	MOV AH, 02
	INT 21h

	MOV AH, 01 	; READ INPUT
	INT 21h 	; AL STORES THE INPUT
	MOV DL, AL 	; TO PRINT THE INPUT
	MOV AH, 02
	INT 21h 	; DO THE PRINTING

	MOV DX, OFFSET VAR ; PRINT A MESSAGE
	MOV AH, 09
	INT 21h

	MOV AH, 4Ch
	MOV AL, 0
	INT 21h

VAR DB 'Hello, World!$'
code ends