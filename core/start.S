global _start; 
global loader ; loader für Linker sichtbar machen
extern init   ; main-Funktion des C-Kernels
 
FLAGS    equ 0
MAGIC    equ 0x1BADB002       ; Magicnumber - Erkennungsmerkmal für GRUB
CHECKSUM equ -(MAGIC + FLAGS) ; Checksum
 
section .text
align 4
MultiBootHeader:
  dd MAGIC       ; Magic number
  dd FLAGS       ; Flags
  dd CHECKSUM    ; Checksum
 

_start:
  mov esp,0x200000 ; Stack an die 2MB-Grenze platzieren
  push eax         ; Multiboot Magicnumber auf den Stack legen
  push ebx         ; Adresse der Multiboot-Structure auf den Stack legen
  
  
  ;call load_gdt
  call init        ; main-Funktion des C-Kernels aufrufen
 
  cli ; falls der Kernel bis hier her kommt, CPU anhalten
  hlt


load_gdt:
	lgdt	[gdtable]		; GDT laden
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	jmp 0x8:.newcs
	.newcs:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;; == GDT == ;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

gdtable:					; Deskriptortabelle
	dw	gdt_end-gdt-1		; Limit
	dd	gdt					; Basis
gdt:
	Null_Desc:
	dd	0					; Nulldeskriptor, bestehend aus Nullen
	dd	0					; Das gleiche. Insgesamt 8 Byte

	Code_Desc:				; Code Deskriptor, erstreckt sich Ã¼ber den ganzen Speicher
	dw	0xFFFF				; SegmentgrÃ¶sse: 0xFFFFF (zusammen mit dem zweitletzten Byte)
	dw	0					; Segmentbasisadresse
	db	0
	db	0x9A
	db	0xCF
	db	0

	Data_Desc:			; Daten Deskriptor, entspricht in etwa dem Code Deskriptor
	dw	0xFFFF
	dw	0
	db	0
	db	0x92
	db	0xCF
	db	0
gdt_end:				; GDT End
