.MODEL  SMALL
.DATA
NL      DB      0DH,0AH,'$'
.CODE
MAIN    PROC
        MOV     AX,@DATA
        MOV     DS,AX
READ_CHAR:
        MOV     AH,01H
        INT     21H
        CMP     AL,0DH
        JE      END_OF_SCAN
        CMP     AL,2BH
        JE      EVAL_PLUS
        CMP     AL,2DH
        JE      EVAL_MINUS
CONV_ASCII:
        CMP     AL,39H
        JLE     CONVERT
        SUB     AL,37H
        JMP     TO_PUSH
CONVERT:
        SUB     AL,30H
TO_PUSH:
        MOV     AH,0
        PUSH    AX
        JMP     READ_CHAR
EVAL_PLUS:
        POP     AX
        POP     BX

        ADD     AX,BX
        PUSH    AX
        JMP     READ_CHAR
EVAL_MINUS:
        POP     AX
        POP     BX

        SUB     AX,BX
        PUSH    AX
        JMP     READ_CHAR
END_OF_SCAN:
        POP     BX
        MOV     CL,4
        MOV     AH,09H
        LEA     DX,NL
        INT     21H


NEXT_HEX:
        MOV     DL,BL
        AND     DL,0F0H
        SHR     DL,CL
       
        CMP     DL,9H
        JLE     ZERO_TO_9
       
        ADD     DL,37H

        JMP     DISPLAY
ZERO_TO_9:
        ADD     DL,30H
DISPLAY:
        MOV     AH,02H
        INT     21H
        SHL     BL,CL
       
         CMP     BL,0H
        JZ      EXIT
        JMP     NEXT_HEX
EXIT:
        MOV     AH,4CH
        INT     21H
MAIN    ENDP
END MAIN


OUTPUT
*****

ENTER BASE:3
ENTER POWER:209
C:\tasm>EX12

ENTER BASE:4
ENTER POWER:4I6
C:\tasm>EX12

ENTER BASE:3
ENTER POWER:103