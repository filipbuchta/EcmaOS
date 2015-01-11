[BITS 16]


PrintString16:
	lodsb
	or	AL, AL
	jz	.end

	mov	AH, 0x0E
	mov	BH, 0x00
	mov	BL, 0x07
	int	0x10
        
	jmp	PrintString16
    .end:
	ret


PrintNumber16:
        mov     [TempRegion], BYTE 0
        mov     di, TempRegion + 1

    .start:
	xor	edx, edx
        mov     eax, ecx
        mov     ebx, 0x10
        div     ebx
        mov     ecx, eax
	xor	eax, eax
	
        mov     ebx, edx ; DX contains remainder
        mov     si, Digits
        mov     al, byte [bx + si] ; Store correct char         
        mov     [di], BYTE al
        inc     di
        
	or	ecx, ecx
	jnz	.start


        dec    di
    
        std
        mov    si, di
    
        call   PrintString16
        cld
        
        mov     si, NewLineConstant
        call    PrintString16

	ret

PrintLine16:
        call    PrintString16
        mov     si, NewLineConstant
        call    PrintString16
        ret