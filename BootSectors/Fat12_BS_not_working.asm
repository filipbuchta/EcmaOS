[BITS 16]
[ORG 0x7C00]

jmp	Step1
	nop

OemName				DB	"ECMAOS  "
BytesPerSector		DW	512
SectorsPerCluster	DB	1
ReservedSectors		DW	1
NumberOfFATs		DB	2
RootEntries			DW	224
TotalSectors		DW	2880
Media				DB	0xF0
SectorsPerFAT		DW	9
SectorsPerTrack		DW	18
HeadsPerCylinder	DW	2
HiddenSectors		DD	0
TotalSectorsBig		DD	0
DriveNumber			DB	0
					DB	0
ExtBootSignature	DB	0x29
SerialNumber		DD	0xa0a1a2a3
VolumeLabel			DB	"MOS FLOPPY "
FileSystem			DB	"FAT12   "

Step1:
	xor	ax, ax
	mov	ds, ax
	mov	es, ax

	mov     ss, ax
	mov     sp, 0400h
	mov     bp, sp


     
	mov	SI, msg_init
	call	PrintLine
	  
    xor     eax, eax
	mov	    al, BYTE [NumberOfFATs]
	mul	    WORD [SectorsPerFAT]
	add	    ax, WORD [ReservedSectors] ; EAX = Starting sector of root directory

    mov     cx, 1 ; number of sectors
	mov	    bx, RootDirectoryRegion ; destination
	call	ReadDisk
	
        
    call    FindStage2
        
	mov	    SI, di
	call	PrintLine
        
    mov     dx, [di + 26]
    push    dx ; Store first sector of Stage2

    xor     eax, eax
    mov     al, [NumberOfFATs]
    mul     WORD [SectorsPerFAT]
    add     ax, WORD [ReservedSectors]
    mov     cx, 1
    mov     bx, RootDirectoryRegion
    call    ReadDisk
        
        	   xchg bx, bx	
   xor     eax, eax
   mov     ax, 1 ; First sector is FAT table
   mov     cx, [SectorsPerFAT]
   mov     bx, FATRegion
   call    ReadDisk
       
	   
   ;mov eax, 0x1000
   ;mov		es, eax
   pop     ax ; Restore first sector of stage 2
   mov     di, Stage2Region
   call    ReadFile
   xchg bx, bx
   ;jmp     0x1000:Stage2Region
   jmp     Stage2Region

   ;mov     si, Stage2Region
   ;call    PrintString
        



;	mov	eax, 33 ; sector to load
;       mov     cx, 1 ; number of sectors
;	mov	BX, 0x8000 ; destination
;	call	ReadDisk
;
;	mov	SI, 0x8000
;	call	PrintString


	cli
	hlt

ReadFile:
        xchg bx, bx
        push    ax
        
        add     ax, 33
        sub     ax, 2
        xor     cx, cx
        mov     cl, BYTE [SectorsPerCluster]
        mov     bx, di
		
        call    ReadDisk

        add     di, [BytesPerSector]; * [SectorsPerCluster]
          
        ; compute address of first sector in fat table
        pop     ax
        push    ax
        
        mov     dx, ax         
        shr     dx, 1
        add     ax, dx
 
       
        mov     cx, FATRegion
        add     cx, ax
        mov     si, cx
        
        mov     cx, WORD [si]
        
        pop     ax
        test    ax, 1
        jnz    .odd
     .even:
        and     cx, 0000111111111111b
        jmp     .done
     .odd:
        shr     cx, 4
        jmp     .done
     .done:
        mov     ax, cx
        cmp     cx, 0x0FF0
		
        jb      ReadFile
        
    ret

FindStage2:
        xor    ecx, ecx
        mov    cx, WORD [RootEntries]
        mov    di, 0x7F00
    .nextEntry:
        push   cx
        mov    cx, 11
        mov    si, Stage2File
        push   di
        repe cmpsb
        pop    di
        pop    cx
        je    .end
        add    di, 32
        loop   .nextEntry
        jmp    .error
    .end:
        ret
    .error:
        hlt


;;;;;;;;;;;;;;;;;;;;
; ReadDisk
;
; Inputs:       eax     = LBA sector to read
;               cx      = number of sectors to read
;               bx      = destination offset
ReadDisk:
	call	DiskReset
    push    cx ; store input for later
    push    bx
    push    eax
    
	xor	dx, dx
	div	WORD [SectorsPerTrack]
	inc	dl
	mov	cl, dl ; sector
 
	xor     dx, dx                              
	div     WORD [HeadsPerCylinder]
	mov     dh, dl ; head
 
	mov	ch, al ; track/cylinder

	mov	ah, 0x02
	mov	al, 1 ; read one sector
	mov	dl, [DriveNumber] ; drive

	int	0x13
	jc	.error

        pop     eax
        pop     bx
        pop     cx
        loop    ReadDisk
	ret
    .error:
	xchg bx, bx
	mov	SI, msg_disk_read_failure
	call	PrintLine
	hlt

DiskReset:
	mov	AH, 0x00
	mov	DL, [DriveNumber]
	int	0x13
	jc	DiskReset
	ret


PrintNumber:
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
    
        call   PrintString
        cld
        
        mov     si, NewLineConstant
        call    PrintString

	ret

PrintLine:
        call    PrintString
        mov     si, NewLineConstant
        call    PrintString
        ret
        
PrintString:
	lodsb
	or	AL, AL
	jz	.end

	mov	AH, 0x0E
	mov	BH, 0x00
	mov	BL, 0x07
	int	0x10
        
	jmp	PrintString
    .end:
	ret


NewLineConstant db 13, 10, 0
msg_init db 'Stage1', 0
msg_disk_read_failure db 'FAILURE', 0

Stage2File		DB	"STAGE2  BIN" 
;Stage2File		DB	"ENDWORLDTXT"
;Stage2File		DB	"HELLOWORTXT"

Digits                  DB      "0123456789ABCDEF"


TempRegion              EQU      0x7E00

RootDirectoryRegion     EQU      0x7F00
FATRegion               EQU      0x8000
Stage2Region            EQU      0x0000   

TIMES 510 - ($ - $$) DB 0
DW 0xAA55