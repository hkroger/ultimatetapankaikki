; IPX EXAMPLE FOR WATCOM C/C++ (10.0a) UNDER DOS4GW PMODE DPMI.
; Written by Daniel Jeppson d94dj@efd.lth.se 960511 03:28 AM (8

; You are hearby granted the right to use this code freely but
; I would appreciate a credit or greet if you find it useful...

; Please see the readme.txt and chat.c for more information...

    .386p
    .model flat,c
    locals
    jumps 

      ; Macros to Decrement Counter & Jump on Condition
LOOPx MACRO Register, Destination
    DEC     Register                ; Counter--
    JNZ     Destination             ; Jump if not 0
ENDM
LOOPjz MACRO Register, Destination
    DEC     Register                ; Counter--
    JZ      Destination             ; Jump if 0
ENDM

PUSHALL MACRO     ; Pushes all registers onto stack = 18c
    PUSH EAX
    PUSH EBX
    PUSH ECX
    PUSH EDX
    PUSH ESI
    PUSH EDI
ENDM

POPALL MACRO      ; Pops all registers from stack = 18c
    POP EDI
    POP ESI
    POP EDX
    POP ECX
    POP EBX
    POP EAX
ENDM

POPALLSEGS MACRO
    POP GS 
    POP FS
    POP ES
    POP DS
ENDM
    
PUSHALLSEGS MACRO
    PUSH DS
    PUSH ES
    PUSH FS
    PUSH GS
ENDM    

SWAPPA MACRO Register   ; Swaps low and high byte in a word...
    ROL     Register,8
ENDM

  assume CS:_TEXT,DS:DGROUP

  dataseg

  global lseg:word
  lseg DW 0
  global lsel:word
  lsel DW 0
    
  global getlowmem:PROC
  global freelowmem:PROC

  codeseg

  getlowmem PROC   ; Get low memory! (BX = paragraphs of memory)
    PUSH EBX       ; AX = segment to memory
    PUSH ECX
    PUSH EDX
    PUSH ESI
    PUSH EDI
    MOV AX,0100h   ; Get BX paragraphs of memory...
    INT 31h        ; (1 paragraph = 16 bytes)
    MOV lseg,AX
    MOV lsel,DX
    ;XOR EAX,EAX
    MOV AX,DX
    POP EDI
    POP ESI
    POP EDX
    POP ECX
    POP EBX
    RET
  getlowmem ENDP

  freelowmem PROC ; Free low memory! (DX = selector)
    PUSHALL
    PUSHALLSEGS
    MOV AX,0101h
    INT 31h
    POPALLSEGS
    POPALL
    RET
  freelowmem ENDP
  
END
