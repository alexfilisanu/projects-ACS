section .data
	format db "%c", 10, 0

section .text
	global cpu_manufact_id
	global features
	global l2_cache_info
	extern printf

;; void cpu_manufact_id(char *id_string);
;
;  reads the manufacturer id string from cpuid and stores it in id_string
cpu_manufact_id:
	enter 	0, 0
	pusha
	xor eax, eax ; eax = 0
	cpuid
	mov eax, [ebp + 8]
	mov [eax], ebx
	mov [eax + 4], edx
	mov [eax + 8], ecx
	popa
	leave
	ret

;; void features(char *vmx, char *rdrand, char *avx)
;
;  checks whether vmx, rdrand and avx are supported by the cpu
;  if a feature is supported, 1 is written in the corresponding variable
;  0 is written otherwise
features:
	enter 	0, 0
	pusha
	mov eax, 1 ; eax = 1
	cpuid
	; valoarea bitului 5
	mov eax, [ebp + 8]
	mov ebx, ecx
	shr ebx, 5
	and ebx, 1
	mov [eax], ebx
	; valoarea bitului 30
	mov eax, [ebp + 12]
	mov ebx, ecx
	shr ebx, 30
	and ebx, 1
	mov [eax], ebx
	; valoarea bitului 28
	mov eax, [ebp + 16]
	mov ebx, ecx
	shr ebx, 28
	and ebx, 1
	mov [eax], ebx

	popa
	leave
	ret

;; void l2_cache_info(int *line_size, int *cache_size)
;
;  reads from cpuid the cache line size, and total cache size for the current
;  cpu, and stores them in the corresponding parameters
l2_cache_info:
	enter 	0, 0
	pusha
	mov eax, 80000006h ; eax = 80000006h
	cpuid
	; bitii 07 - 00
	mov eax, [ebp + 8]
	mov ebx, ecx
	and ebx, 0xff
	mov [eax], ebx
	; bitii 31 - 16
	mov eax, [ebp + 12]
	mov ebx, ecx
	shr ebx, 16
	and ebx, 0xffff
	mov [eax], ebx
	
	popa
	leave
	ret
