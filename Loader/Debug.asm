    
ClearScrean:
    pusha
    cld
    mov    edi, VIDEO_MEMORY
    mov    cx, 2000
    mov    ah, 14
    mov    al, ' '
    rep    stosw
    mov    byte [CursorX], 0
    mov    byte [CursorY], 0
    
    
    popa
    ret
    
PrintLine:
    pusha
    call PrintString

    mov byte [CursorX], 0    
    inc byte [CursorY]
    
    popa
    ret
    
PrintString:
    pusha
    
    push ebx
    pop  edi
    
    .loop:
        mov bl, byte [edi]
        cmp bl, 0
        je .done
        
        
        call PrintChar
        
        inc edi
        jmp .loop
    
    .done:
        popa
        ret

PrintChar:
    pusha
    
    mov edi, VIDEO_MEMORY
    
    xor eax, eax
    mov ecx, COLS * 2
    mov al, byte [CursorY]
    mul ecx
    push eax
  
    xor eax, eax  
    mov al, byte [CursorX]
    mov ecx, 2
    mul ecx
    pop ecx
    add eax, ecx
    
    add edi, eax
    
    mov dl, bl
    mov dh, 14
    mov word [edi], dx 
    
    inc byte [CursorX]
    cmp byte [CursorX], COLS
    je .line
    jmp .done
    
    .line:
        mov byte [CursorX], 0
        inc byte [CursorY]
        jmp .done    
    .done: 
        popa
        ret
    
VIDEO_MEMORY equ 0xb8000
COLS equ 80
LINES equ 25
CHAR_ATTRIBUTE equ 14

CursorX db 0
CursorY db 0
