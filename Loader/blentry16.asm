[BITS 16]
[org 0x8000]

entry:
    jmp Start

%include "Debug16.asm"

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0
 
gdt_code_segment:
	dw 0FFFFh 			; limit low
	dw 0 				; base low
	db 0 				; base middle 
	db 10011010b 			; access
	db 11001111b 			; granularity
	db 0 				; base high

gdt_ds:
        dw 0FFFFh 			; limit low (Same as code)
	dw 0 				; base low
	db 0 				; base middle
	db 10010010b 			; access
	db 11001111b 			; granularity
	db 0				; base high

gdt_end:

gdtr:
	dw gdt_end - gdt_start - 1 ; GDT size
	dd gdt_start

CODE_SEG equ gdt_code_segment - gdt_start
DATA_SEG equ gdt_ds - gdt_start

Start:

	
	;cli
	;mov		ax, cs
	;mov		ds, ax
	;mov		es, ax
	;mov		ss, ax
	
	
	mov	SI, msg_init
	call	PrintLine16
        

        ; Enable A20
        mov ax, 0x2401
        int 0x15
        jc  Failure

        mov     si, MessageA20Enabled
        call    PrintLine16
        
        cli
        
        lgdt [gdtr]
        
		xor     ax, ax
        mov     ds, ax
        mov     es, ax
        mov     ss, ax
        mov     fs, ax
        mov     gs, ax
		
		mov eax, cr0
        or al, 1
        mov cr0, eax
        
		; enable sse
		mov eax, cr0
		and ax, 0xFFFB			; clear coprocessor emulation CR0.EM
		or ax, 0x2				; set coprocessor monitoring  CR0.MP
		mov cr0, eax
		mov eax, cr4
		or ax, 3 << 9			; set CR4.OSFXSR and CR4.OSXMMEXCPT at the same time
		mov cr4, eax
		

		MOV eax, CR0                           
		AND eax, ~6      
		MOV CR0, eax                           
		FNINIT                                 


        jmp 0x08: init_pmod


	cli
	hlt


Failure:
        mov     si, MessageFailure
        call    PrintLine16
        hlt


        


NewLineConstant db 13, 10, 0

Digits                  DB      "0123456789ABCDEF"
TempRegion              EQU      0x7E00

msg_init db 'Stage2', 0
MessageFailure db "Failure", 0
MessageA20Enabled db "A20 enabled", 0


[BITS 32]
init_pmod:
    mov     ax, 0x10
    mov     ds, ax
    mov     ss, ax
    mov     es, ax
    mov     esp, 0x90000

     
     ; Prepare page tables

   ;    xor     eax, eax
   ;    mov     edi, PM_PDPT_ADDRESS
   ;    mov     ecx, 0400h
   ;    rep stosd

   ;    xor     eax, eax
   ;    mov     edi, PM_PDT_ADDRESS
   ;    mov     ecx, 0400h
   ;    rep stosd
       
   ;    mov     eax, PM_PDPT_ADDRESS
   ;    mov     dword [eax], PM_PDT_ADDRESS | PTE_PRESENT

   ;    mov     eax, PM_PDT_ADDRESS
   ;    mov     dword [eax], 0 | PTE_PRESENT | PTE_WRITEABLE | PTE_ACCESSED | PTE_2MB


   ; Enable PSE

   ;    mov     eax, cr4
   ;    or      eax, CR4_PSE | CR4_PAE | CR4_PCE | CR4_OSFXSR
   ;    mov     cr4, eax
       
    ; Set root Page Table
       
   ; mov     eax, PM_PDPT_ADDRESS
   ; mov     cr3, eax
       
       
       
    call ClearScrean

    mov ebx, msg_32_init
    call PrintLine


    call   BlLoadImage


    call   eax


;    mov     ebx, ValidMessage
;    call    PrintLine
  
  
    
    hlt


BlLoadImage:

    mov    ebp, BlImageStart
    cmp    word [ebp], 0x5A4D ; "MZ" Check exe magic number
    jne    .invalidImage
   
    
    mov    ebx, [ebp + 60] ; e_lfanew
    add    ebx, ebp
   
    cmp    dword [ebx], 0x000004550 ; "PE\0\0"
    jne    .invalidImage
  
      
    ; 24 - PE Header
    ; 28 - offset to image base
    cmp    dword [ebx + 24 + 28], IMAGE_ADDRESS ; Compare image base with expected address
    ;mov eax, dword [ebx + 24 + 28]
    ;xchg bx, bx
    jne    .invalidImage
   
   
    
    mov    esi, ebp
    mov    edi, IMAGE_ADDRESS
    mov    ecx, dword [ebx + 24 + 60]
    rep movsb
   
    xor     esi, esi
    mov     si, word [ebx + 20] ; Size of optional header
    add     esi, 24
    add     esi, ebx
    
    xor     ecx, ecx
    mov     cx, word [ebx + 6] ; Number of sections
    
  
    ; Copy all sections
    .loop:
        push    ecx
   
        mov     ecx, ebp
        mov     edx, esi
        
       
        push    ebp
        mov     ebp, esp
        push    esi
        push    edi
       
        
        
        mov     esi, dword [edx + 0x14] ; Offset to raw data
        add     esi, ecx

        mov     edi, dword [edx + 0x0C] ; Offset to virtual address
        add     edi, IMAGE_ADDRESS
        
        push    esi
        push    edi
        
        ; Clear target address with zeros
        mov     eax, 0
        mov     ecx, dword [edx + 0x08] ; Offset to virtual size
        rep stosb
        
        pop     edi
        pop     esi
       
        mov     ecx, dword [edx + 0x08]
        cmp     ecx, dword [edx + 0x10]
        jl      .bigger
        mov     ecx, dword [edx + 0x10]
        .bigger:
        
        rep movsb
        
        pop     edi
        pop     esi
        mov     esp, ebp
        pop     ebp
        
        pop     ecx
        add     esi, 0x28 ; Section header size
        dec     ecx
        jnz .loop

      
    mov    eax, dword [ebx + 0x18 + 0x10]
    add    eax, IMAGE_ADDRESS
    
    ret

    .invalidImage:
        mov ebx, ErrorMessage
        call PrintLine
        
        hlt




%include "Debug.asm"
  
%include "Fat12.asm"

 
msg_32_init db "PMode entered", 0   
ValidMessage db "Valid", 0   
 
ErrorMessage db "Error", 0   

LM_PML4T_ADDRESS        equ     000021000h
PM_PDPT_ADDRESS         equ     000022000h
PM_PDT_ADDRESS          equ     000023000h

PTE_PRESENT             equ     000000001h
PTE_WRITEABLE           equ     000000002h
PTE_ACCESSED            equ     000000020h
PTE_2MB                 equ     000000080h

IMAGE_ADDRESS           equ     0x40000

CR4_PSE                 equ     000000010h
CR4_PAE                 equ     000000020h
CR4_PCE                 equ     000000100h
CR4_OSFXSR              equ     000000200h

BlImageStart:
