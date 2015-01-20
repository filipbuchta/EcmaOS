[BITS 16]
[ORG 0x7C00]

	jmp	Step1
        nop

OemName			DB	"ECMAOS  "
BytesPerSector		DW	512
SectorsPerCluster	DB	1
ReservedSectors		DW	1
NumberOfFATs		DB	2
RootEntries		DW	224
TotalSectors		DW	2880
Media			DB	0xF0
SectorsPerFAT		DW	9
SectorsPerTrack		DW	18
HeadsPerCylinder	DW	2
HiddenSectors		DD	0
TotalSectorsBig		DD	0
DriveNumber		DB	0
				DB	0
ExtBootSignature	DB	0x29
SerialNumber		DD	0xa0a1a2a3
VolumeLabel		DB	"MOS FLOPPY "
FileSystem		DB	"FAT12   "

Step1:

	xor		ax, ax
	mov	    ds, ax
	mov	    es, ax
	mov     ss, ax
	mov     sp, 0400h
	mov     bp, sp



	; Clear screen
	mov     ax, 0x02
	int     0x10



	; Save boot drive
	mov		[DriveNumber], dl
	


	mov	SI, InitMsg
	call	PrintString



	; Check for int 13 extensions

	mov     ah, 0x41
	mov     bx, 0x55aa
	int     0x13
	jc		HWUnsupportedError
	cmp     bx, 0xaa55
	jne     HWUnsupportedError




	; TODO: this computation should not be done from sample values, but from actual values on disk
    xor     eax, eax
	mov		al, BYTE [NumberOfFATs]
	mul		WORD [SectorsPerFAT]
	add		ax, WORD [ReservedSectors] ; EAX = Starting sector of root directory

	mov     bx, 0x050
	mov		es, bx           ; Read to 0050:0000
	mov     cx, 1			 ; 1 Sector
	xor		di, di
	call	ReadDisk

	mov		bx, 0x0050
	mov		ds, bx
	mov		ax, 0
	mov		es, ax
	mov		si, ax

    call    FindStage2

    mov		bx, 0
	mov		ds, bx
	

	push    ax ; Store first sector of Stage2
		
	
    xor     eax, eax
    mov     ax, 1 ; First sector is FAT table
    mov     cx, [SectorsPerFAT]
    mov     bx, 0x0050
	mov		es, bx	; Read to 0000:0500
	xor		di, di
    call    ReadDisk

	xor		eax, eax
	mov		ds, eax
	mov		si, ax

	; load to 0800:0000

	mov	    eax, 0x0800
	mov     es, eax
	xor		di, di
 
    pop     ax ; Restore first sector of stage 2
    

	
	
    call    ReadFile
    

	xor		eax, eax
	mov		es, eax
	mov		ds, eax


    jmp     0x0000:0x8000

        

	cli
	hlt



;;;;;;;;;;;;;;;;;;;;
; ReadFile
;
; Input		   ax    = First LBA to read
;			   es:di = Destination address
ReadFile:
		
        push    ax
        
        add     ax, 33			; boot sector + 2x fat + root directory ( 1 + 9 + 9 + 14 )
        sub     ax, 2			; clusters starts from 2
        mov		cx, 1			; [SectorsPerCluster]
        call    ReadDisk
       
		mov		edx, es
		add		edx, 0x20
        mov     es, edx		    ; TODO: ([BytesPerSector] * [SectorsPerCluster]) >> 1
        
		xor eax, eax  
        ; compute address of first sector in fat table
        pop     ax
        push    ax
        
        mov     dx, ax         
        shr     dx, 1
        add     ax, dx

		push ds 
       
		mov		ecx, 0x050
		mov		ds, ecx

        xor		ecx, ecx
        add     cx, ax
        mov     si, cx
        
        mov     cx, WORD [ds:si]

		pop		ds

        
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

;;;;;;;;;;;;;;;;;;;;
; FindStage2
;
; Input		   ds:si = Address of first entry
; Outputs	   ax = Address of first cluster

FindStage2:
        xor    ecx, ecx
        mov    cx, WORD [RootEntries]
    .nextEntry:
        push   cx
        mov    cx, 11
        mov    di, Stage2File
        push   si
        repe cmpsb
		pop    si
        pop    cx
        je    .found
        add    si, 32
        loop   .nextEntry
        jmp    .error
    .found:
		mov     ax, [ds:si+26]
        ret
    .error:
		call HWUnsupportedError
        hlt

;;;;;;;;;;;;;;;;;;;;
; ReadDisk
;
; Inputs:       eax     = LBA sector to read
;               cx      = number of sectors to read
;               es:di   = destination

ReadDisk:
	pushad
	

	
	mov	dl, [DriveNumber]

	; Construct packet

	push	DWORD 0x00
	push    DWORD eax   ; soruce address

	push    WORD es
	push	WORD 0x00   ; destination address

	push    WORD cx     ; number of sectors

	push	WORD 0x10   ; packet size


	mov     ah, 0x42
	mov     si, sp
	
	int     0x13

	
	add     sp, 0x10
	popad

	ret

;;;;;;;;;;;;;;;;;;;;
; PrintString
;
; Inputs:       si     = Address of string to print

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


HWUnsupportedError:
	mov     si, ErrorMsg
    call    PrintString
	mov     si, HwErrorMsg
    call    PrintString

	hlt



InitMsg db 'Stage1', 13,10, 0
msg_disk_read_failure db 'FAILURE', 13,10, 0

HwErrorMsg db 'HW', 0
ErrorMsg db 'Error: ', 0

Stage2File		DB	"STAGE2  BIN"




TIMES 510 - ($ - $$) DB 0
DW 0xAA55