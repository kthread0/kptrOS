global enablePaging

enablePaging:
    push rbp
    mov ebp, esp
    mov rax, cr0
    or rax, 0x80000000
    mov cr0, rax
    mov esp, ebp
    pop rbp
    ret
