; This is your structure
struc  my_date
    .day: resw 1
    .month: resw 1
    .year: resd 1
endstruc

section .text
    global ages

; void ages(int len, struct my_date* present, struct my_date* dates, int* all_ages);
ages:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]  ; len
    mov     esi, [ebp + 12] ; present
    mov     edi, [ebp + 16] ; dates
    mov     ecx, [ebp + 20] ; all_ages
    ;; DO NOT MODIFY

    ;; TODO: Implement ages
    ;; FREESTYLE STARTS HERE

    xor ebx, ebx ; il folosesc pentru a parcurge atat vectorul dates
                 ; cat si pentru a scrie in all_ages
    xchg edx, ecx ; interschimb edx si ecx pentru a putea folosi loop 

age:
    mov eax, [esi + 4]
    sub eax, [edi + ebx * 2 + 4] ; scad din anul present, anul nasterii
    mov [edx + ebx], eax         ; salvez rezultatul in vectorul all_ages
                                 ; pe pozitia aferenta
    ; daca luna present este >= luna nasterii
    mov ax, [esi + 2] 
    sub ax, [edi + ebx * 2 + 2] ; scad din luna present, luna nasterii
    cmp ax, 0   ; daca rezultatul < 0 => scad 1 an din vectorul all_ages 
    jl decage1
    cmp ax, 0   ; daca rezultatul == 0 => verific zilele din present si dates
    je decage2
increase:
    add ebx, 4  ; avans contor
    loop age

    xchg ecx, edx ; interschimb la loc pentru ca vectroul all_ages sa fie in ecx

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY

decage1:
    mov eax, [edx + ebx]
    dec eax ; scad 1 din vectorul all_ages
    cmp eax, 0  ; daca varsta < 0 => fac varsta din all_ages 0
    jl make0
continue:
    mov [edx + ebx], eax
    jmp increase

decage2:
    mov ax, [esi]
    sub ax, [edi + ebx * 2]  ; scad din ziua present, ziua nasterii
    cmp ax, 0   ; daca rezultatul < 0 => scad 1 an din vectorul all_ages
    jl decage1
    jmp increase

make0:
    mov eax, 0
    jmp continue