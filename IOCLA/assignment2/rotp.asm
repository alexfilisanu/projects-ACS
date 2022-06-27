section .text
    global rotp

;; void rotp(char *ciphertext, char *plaintext, char *key, int len);
rotp:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]  ; ciphertext
    mov     esi, [ebp + 12] ; plaintext
    mov     edi, [ebp + 16] ; key
    mov     ecx, [ebp + 20] ; len
    ;; DO NOT MODIFY

    ;; TODO: Implment rotp
    ;; FREESTYLE STARTS HERE

    xor ebx, ebx ; in ebx salvez un contor pentru a parcurge plaintext 

encryption: 
    ; folosesc registru al in loc de eax pentru ca salvez un char in el
    mov al, [esi + ebx] 
    xor al, [edi + ecx - 1]
    mov [edx + ebx], al 
    
    inc ebx

    loop encryption

    ;; FREESTYLE ENDS HERE
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY