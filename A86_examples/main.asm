JMP START

; burda degiskenler
	number db 5 dup 0		; stores output string
	cr     dw 10, 13, "$"			; carriage return, line feed
	temp   dw 0
	n1     dw 0				; stores first number
	n2     dw 0				; stores second number


START:
	mov cx, 0

MORECHAR:
	mov ah, 01h
	int 21h
	mov dx, 0 ; dx becomes 0
	mov dl, al ; al is where input char is stored, we move it to the dl.
	mov ax, cx
	
	
	
	cmp dl, 20h ;if character is space, new number is coming so add the current number to stack.
	je FINNUM
	
	cmp dl, 2bh
	je ADDITION

	cmp dl, 2ah
	je MULT
	
	cmp dl, 2fh
	je DIVISION
	
	cmp dl, 5eh
	je BITXOR
	
	cmp dl, 7ch
	je BITOR
	
	cmp dl, 26h
	je BITAND
	
	cmp dl, 0D
	je TEMP_FINISH

	cmp dx, '9'
	je NORMAL
	jl NORMAL
	ja HARF


NORMAL:
	sub dx,'0'				; convert from ascii to real numeric value
	jmp BITTI
HARF:
	sub dx, 55
BITTI:

	mov temp,dx				; 3 lines later, when we multiply ax and cx, dx changes. so, copy dx's value to temp
	mov ax,cx		
	mov cx,16d
	mul cx					; multiply ax by 16 (shift the number read so far) 320 + 5  
	add ax,temp				; add numeric value of new input character to the number read so far
	mov cx,ax				; put result to cx again
	jmp MORECHAR

FINNUM:
	;add cx to stack
	push cx
	mov cx, 0
	jmp MORECHAR


TEMP_FINISH:
	push cx
	jmp FINISH

ADDITION:
	;;; ADD two numbers on stack
	pop bx
	pop cx
	add cx, bx
	push cx
	jmp TEST_FINISH

MULT:
	;;; multiply two numbers on stack
	pop ax
	pop cx
	mul cx
	push ax
	jmp TEST_FINISH

DIVISION:
	mov dx, 0
	pop cx
	pop ax
	div cx
	push ax
	jmp TEST_FINISH
BITXOR:
	pop bx
	pop cx
	XOR cx, bx
	push cx
	jmp TEST_FINISH

BITAND:
	pop bx
	pop cx
	AND cx, bx
	push cx
	jmp TEST_FINISH

BITOR:
	pop bx
	pop cx
	OR cx, bx
	push cx
	jmp TEST_FINISH

TEST_FINISH:
	mov ah, 01h
	int 21h
	mov dl, al ; al is where input char is stored, we move it to the dl.
	cmp dl, 0D
	je FINISH
	mov cx, 0
	jmp MORECHAR


FINISH:
	; Prints the last number in stack and exits.
	pop ax
;=====================
; convert number to string
;=====================


setup_string:
	;result is already in ax
	mov bx,offset number+4 	; put a $ at end of buffer
	mov b[bx],"$"			; we will fill buffer from back
	dec bx

convert_decimal:
	mov dx,0
	mov cx,16d
	div cx					; divide ax (i.e. current number) by 10 to get the last digit

	cmp dx, 9
	je NORMAL_YAZ
	jl NORMAL_YAZ
	ja HARF_YAZ
	
NORMAL_YAZ:
	add dx,48d 				; convert remainder (last digit) to its ASCII representation
	jmp BITTI_YAZ
HARF_YAZ:
	add dx, 55d
BITTI_YAZ:
 			
	mov [bx],dl				; and move to buffer for output
	dec bx
	cmp ax,00				; check if we have got all digits
	jnz convert_decimal


print_cr:
	mov ah,09
	mov dx,offset cr		; print out a carriage return character
	int 21h

printout:
	mov dx,bx				; give the address of string to dx
	inc dx					; we decremented once too many, go forward one
	mov ah,09
	int 21h

exit:
	mov ah,04ch				; exit the program with 0
	mov al,00
	int 21h