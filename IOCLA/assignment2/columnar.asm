section .data
    extern len_cheie, len_haystack
    counter_key dd 0
    counter_plain dd 0
    nr_steps dd 0
    nr_steps2 dd 0
    len_plain dd 0
    key_value dd 0
    counter_rez dd 0

section .text
    global columnar_transposition

;; void columnar_transposition(int key[], char *haystack, char *ciphertext);
columnar_transposition:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha 

    mov edi, [ebp + 8]   ;key
    mov esi, [ebp + 12]  ;haystack
    mov ebx, [ebp + 16]  ;ciphertext
    ;; DO NOT MODIFY

    ;; TODO: Implment columnar_transposition
    ;; FREESTYLE STARTS HERE

    mov [counter_key], dword 0 ; counter_key e folosit pentru a parcurge vectorul key
    mov [counter_rez], dword 0 ; counter_rez e folosit pentru a parcurge vectorul haystack

    xor edx, edx
    mov eax, [len_haystack]
    mov ecx, [len_cheie]
    mov [nr_steps], ecx   ; nr_steps = numarul de iteratii din primul loop (coloane)
    div ecx
    add eax, 1
    mov [len_plain], eax   ; numarul de linii: len_haystack / len_cheie + 1

throughColumns:
    mov ecx, [counter_key]
    mov eax, [edi + ecx]
    mov [key_value], eax    ; in key_value pastrez valoarea cheii
    mov [counter_plain], dword 0 ; counter_plain e folosit pentru a parcurge liniile matricii 
    mov eax, [len_plain]
    mov [nr_steps2], eax    ; nr_steps2 = numarul de iteratii din al doilea loop (linii)

throughLines:
    ; forumula pentru parcurgerea haystack:
    ; counter_plain (0 : nr linii - 1) * len_cheie + key_value  
    mov eax, [counter_plain]
    mov ecx, [len_cheie]
    mul ecx
    add eax, [key_value]
 
    cmp eax, [len_haystack] ; daca valoarea obtinuta dupa aplicarea formulei depaseste
    jge jumpOver            ; len_haystack => nu exista caracterul in haystack => sar peste 
    
    mov al, [esi + eax] ; atribui pe pozitia aferenta din ciphertext caracterul din haystack
    mov ecx, [counter_rez]
    mov [ebx + ecx], al
    inc ecx
    mov [counter_rez], ecx
    
jumpOver:
    mov ecx, [counter_plain]  ; avans aferent celui de al doilea loop
    inc ecx
    mov [counter_plain], ecx
    mov ecx, [nr_steps2]
    dec ecx
    mov [nr_steps2], ecx

    cmp ecx, 0
    jne throughLines

    mov ecx, [counter_key]  ; avans aferent primului loop
    add ecx, 4
    mov [counter_key], ecx
    mov ecx, [nr_steps]
    dec ecx
    mov [nr_steps], ecx
    
    cmp ecx, 0
    jne throughColumns

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY