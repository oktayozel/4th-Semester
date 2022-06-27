; a program that inputs 16-bit integers and outputs the maximum of them
jmp start
;=====================
; variables
;=====================
; 5 dup 0 = 0 0 0 0 0
	number db 5 dup 0		; stores output string
	cr     dw 10, 13, "$"			; carriage return, line feed
	temp   dw 0
	n1     dw 0				; stores first number
	n2     dw 0				; stores second number
;===================s

start:
	mov cx,0	 			; cx will hold the current integer
	mov bl,0      			; bl will be used as counter

;=====================
; read characters
;=====================

morechar:
	mov ah,01h				; reads a character to al
	int 21h					
	mov dx,0				; dx becomes 0 
	mov dl,al				; store the input character in dl
	mov ax,cx
	cmp dl,0D				; check if the character is enter (array is over)
	je finnum				
	sub dx,'0'				; convert from ascii to real numeric value
	mov temp,dx				; 3 lines later, when we multiply ax and cx, dx changes. so, copy dx's value to temp
	mov ax,cx		
	mov cx,10d
	mul cx					; multiply ax by 10 (shift the number read so far) 320 + 5  
	add ax,temp				; add numeric value of new input character to the number read so far
	mov cx,ax				; put result to cx again
	jmp morechar

finnum:
	inc bl					; increment counter
	cmp bl, 1				; check if first number is over
	je firstnum
	cmp bl, 2				; check if second number is over
	je secondnum

firstnum:
	mov n1,cx				; put first number to n1 
	mov cx,0				; we will release cx to read the next number
	jmp morechar

secondnum:
	mov n2, cx

summation:
	mov ax, n1
	mov bx, n2
	add ax, bx


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
	mov cx,10d
	div cx					; divide ax (i.e. current number) by 10 to get the last digit
	add dx,48d  			; convert remainder (last digit) to its ASCII representation
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