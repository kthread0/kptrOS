section .text
global timer
extern count

timer:
    push rax
    mov rax, [count]
    test rax, rax
    jz timer
    dec rax
    mov [count], rax
done:
    pop rax
    iretd
