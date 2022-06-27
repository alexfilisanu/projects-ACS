struc node
	val: resd 1
	next: resd 1
endstruc

section .text
	global sort

; struct node {
;     	int val;
;    	struct node* next;
; };

;; struct node* sort(int n, struct node* node);
; 	The function will link the nodes in the array
;	in ascending order and will return the address
;	of the new found head of the list
; @params:
;	n -> the number of nodes in the array
;	node -> a pointer to the beginning in the array
; @returns:
;	the address of the head of the sorted list
sort:
	enter 0, 0	
	
	mov ecx, [ebp + 8] ; = n
	xor ebx, ebx ; il folosesc drept contor
	add ebx, 1
	
	; tinand cont de faptul ca nuemerele se afla in vector in ordine crescatoare
	; (1, 2, 3, ..., n), caut prima data elementul 1, dupa care elemtul 2 + creez
	; legatura intre ele, apoi caut elementul 3 + legatura etc.

	xor edx, edx ; folosesc edx pt parcurgere
	mov edx, [ebp + 12] ; inceputul vectorului

search1:
	cmp [edx + val], dword 1 ; cand am gasit elementul 1 => first_element
	je first_element
	add edx, 8
	loop search1

stop:
	leave
	ret

first_element:
	mov eax, edx ; in registrul eax pastrez inceputul listei, apoi caut
				 ; automat urmatorul element
search_next:      
	inc ebx 
	mov esi, [ebp + 12] ; folosesc esi pentru noua parcurgerea a vectorului
search:
	cmp [esi + val], ebx ; dupa ce gasesc urmtaorul element => make_link
	je make_link
	add esi, 8
	jmp search
make_link:
	mov [edx + next], esi
	mov edx, esi 
	cmp [ebp + 8], ebx ; cand ajung la ultimul element == n => stop
	je stop

	jmp search_next
