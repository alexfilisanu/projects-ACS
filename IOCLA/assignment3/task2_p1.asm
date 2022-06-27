section .text
	global cmmmc

;; int cmmmc(int a, int b)
;
;; calculate least common multiple fow 2 numbers, a and b

	; calculez la inceput a * b, apoi cmmdc ul numerelor, iar la final cmmmc ul
	; o sa fie = a * b  / cmmdc(a, b) 
cmmmc:						
	push ebp
	push esp
	pop ebp

	push dword [ebp + 8]
	pop ebx               ; salvez primul numar in ebx
	push dword [ebp + 12]
	pop ecx               ; salvez al doilea numar in ecx
	xor edx, edx
	mul ecx            
	push eax
	pop edi               ; in edi o sa ramana salvat produsul numerelor

cmmdc:
	cmp ecx, 0            ; while (b) {
	je stop
	xor edx, edx
	push ebx
	pop eax  
	div ecx
	push ecx
	pop ebx               ; a = b;
	push edx
	pop ecx               ; b = a % b;
	jmp cmmdc             ; } => cmmdc va fi in ebx
 
 stop:
	xor edx, edx
	push edi
	pop eax
	div ebx               ; => eax = eax(=a*b) / ebx(=cmmdc(a,b))
		
	push ebp
	pop esp
	pop ebp
	ret
