section .text
	global par

;; int par(int str_length, char* str)
;
; check for balanced brackets in an expression
par:
	push ebp
	push esp
	pop ebp
	
	push dword [ebp + 8]
	pop ecx 	        ; str_length
    push dword [ebp + 12] 
	pop ebx             ; str
	xor eax, eax 		; daca e '(' adaug 1 la eax, daca e ')' scad 1 din eax
 	xor edx, edx		; pentru parcurgerea str

while:
	cmp byte [ebx + edx], '(' 
	je one 						; adaug 1
	cmp byte [ebx + edx], ')'	
	je zero						; scad 1
continue:
	inc edx
	loop while

	push 1		; daca s-a ajuns in acest punct inseamna ca fiecarei '(' ii 
	pop eax		; corespunde una ')' => rezultat final = 1 

stop:
	push ebp
	pop esp
	pop ebp
	ret

one:
	inc eax
	jmp continue

zero:
	dec eax
	cmp eax, 0		; daca eax < 0 => mai multe paranteze inchise decat deschise
	jl make0		; => imposibil => opresc executia si il fac pe eax = 0
	jmp continue

make0:
	xor eax, eax
	jmp stop
