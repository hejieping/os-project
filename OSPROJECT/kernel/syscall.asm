
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               syscall.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

%include "sconst.inc"

INT_VECTOR_SYS_CALL equ 0x90
_NR_get_ticks       equ 0
_NR_write	    equ 1
_NR_clear           equ 2
_NR_getcharfromtty  equ 3
_NR_write_command   equ 4
; 导出符号
global	get_ticks
global	write
global 	clear
global  getcharfromtty	
global  write_command 
bits 32
[section .text]

; ====================================================================
;                              get_ticks
; ====================================================================
get_ticks:
	mov	eax, _NR_get_ticks
	int	INT_VECTOR_SYS_CALL
	ret

; ====================================================================================
;                          void write(char* buf, int len);
; ====================================================================================
write:
        mov     eax, _NR_write
        mov     ebx, [esp + 4]
        mov     ecx, [esp + 8]
        int     INT_VECTOR_SYS_CALL
        ret
; ====================================================================================
;                          int clear()
; ====================================================================================*
clear:
       mov     eax, _NR_clear
       int     INT_VECTOR_SYS_CALL
       ret
; ====================================================================================
;                          char getcharfromtty
; ====================================================================================*
getcharfromtty:
	mov     eax, _NR_getcharfromtty
       int     INT_VECTOR_SYS_CALL
       ret
; ====================================================================================
;                         void  write_command
; ====================================================================================*
write_command:
       mov     eax, _NR_write_command
       int     INT_VECTOR_SYS_CALL
       ret

