jmp readloop                                           
readloop:
                                            
	MOV AH, 01h; to read character
	INT 21h
	CMP AL, 0dh ; check if it is enter
	JE stopreading
	MOV AH, 0 ; make the top half zero 
	push AX ; push AX to stack
	jmp readloop

stopreading:
	;we will print new line here, different approach than previous one
	MOV AH, 02h ; print a character
	MOV DL, 0Dh ; move the cursor to start of the current row, meaning /r
	INT 21h
	MOV DL, 0Ah ; move the cursor to the next line without changing the column, meaning /n. Also we don’t have to put 02h in AH again because it already is.
	INT 21h
printloop:
	CMP SP, 0FFFEh ; check if the stack is empty or not
	JE exit ; if equal jump to exit
	MOV AH, 02h ; print a character
	POP DX ; put a character in DL by popping value from stack in DX because top half is empty
	INT 21h
	JMP printloop
exit:
	MOV AH, 04Ch
	MOV AL, 00
	INT 21h