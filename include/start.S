global _start; 
extern main   ; main-Funktion des C-Kernels

_start:
	push ebx
	push eax
	call main
