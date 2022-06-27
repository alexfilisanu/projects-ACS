global get_words
global compare_func
global sort

section .data
    delim db ` ,.\n`, 0

section .text
    extern strtok
    extern qsort
    extern strcmp
    extern strlen

    ; int compare_func(const void *a, const void *b)
    ; folosita ca parametrul pentru apelul qsort din sort
compare_func:
    enter 0, 0
    push ebx
    mov eax, [ebp + 8]
    mov eax, [eax]   
    push eax
    call strlen
    add esp, 4
    mov ebx, eax     ; ebx = strlen(*(char**)a) 
    
    mov eax, [ebp + 12]
    mov eax, [eax]
    push eax
    call strlen      ; eax = strlen(*(char**)b)
    add esp, 4
    
    cmp ebx, eax     ; <=> if (strlen(*(char**)a) != strlen(*(char**)b)) 
    jne noequal
                     ; <=> if equal =>  
    mov eax, [ebp + 12]
    mov ebx, [eax]
    mov eax, [ebp + 8]
    mov eax, [eax]
    push ebx
    push eax
    call strcmp      ; => return: strcmp((*(char**)a), (*(char**)b))
    add esp, 8
    jmp final
noequal:
    sub ebx, eax     ; => return: strlen(*(char**)a) - strlen(*(char**)b) 
    mov eax, ebx       
    
 final:      
    mov ebx, [ebp - 4]
    leave
    ret

;; sort(char **words, int number_of_words, int size)
;  functia va trebui sa apeleze qsort pentru soratrea cuvintelor 
;  dupa lungime si apoi lexicografix
sort:
    enter 0, 0
    mov edx, [ebp + 16]  ; = size
    mov eax, [ebp + 12]  ; = number_of_words
    push compare_func
    push edx
    push eax
    push dword [ebp + 8]
    call qsort
    add esp, 16

    leave
    ret

;; get_words(char *s, char **words, int number_of_words)
;  separa stringul s in cuvinte si salveaza cuvintele in words
;  number_of_words reprezinta numarul de cuvinte
get_words:
    enter 0, 0
    mov ebx, [ebp + 8]  ; = char *s 
    mov edx, [ebp + 12] ; = char **words
    mov edi, [ebp + 16] ; = int number_of_words
    lea esi, [edx] ; cu esi parcurg array-ul de string-uri words

    push delim      ; primul apel al functiei strtok
    push ebx
    call strtok
    add esp, 8
    mov [esi], eax  ; <=> words[0] = strtok(s, delim)
    add esi, 4
    dec edi

while:              ; urmatoarele apeluri
    push delim
    push 0
    call strtok
    add esp, 8
    mov [esi], eax  ; <=> words[1 : n-1] = strtok(NULL, delim)
    add esi, 4
    dec edi
    cmp edi, dword 0
    jne while
    
    leave
    ret
