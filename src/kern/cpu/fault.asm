extern interrupt_handler

global isr_stub_table
%assign i 0
%rep 48
    global isr_%[i]
%endrep

section .text

%macro ISR_NO_EC 1
isr_%1:
    push 0
    push %1
    jmp common_stub
%endmacro

%macro ISR_EC 1
isr_%1:
    push %1
    jmp common_stub
%endmacro

common_stub:
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call interrupt_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16

    iretq

ISR_NO_EC  0
ISR_NO_EC  1
ISR_NO_EC  2
ISR_NO_EC  3
ISR_NO_EC  4
ISR_NO_EC  5
ISR_NO_EC  6
ISR_NO_EC  7
ISR_EC     8
ISR_NO_EC  9
ISR_EC    10
ISR_EC    11
ISR_EC    12
ISR_EC    13
ISR_EC    14
ISR_NO_EC 15
ISR_NO_EC 16
ISR_EC    17
ISR_NO_EC 18
ISR_NO_EC 19
ISR_NO_EC 20
ISR_EC    21
ISR_NO_EC 22
ISR_NO_EC 23
ISR_NO_EC 24
ISR_NO_EC 25
ISR_NO_EC 26
ISR_NO_EC 27
ISR_NO_EC 28
ISR_EC    29
ISR_EC    30
ISR_NO_EC 31

ISR_NO_EC 32
ISR_NO_EC 33
ISR_NO_EC 34
ISR_NO_EC 35
ISR_NO_EC 36
ISR_NO_EC 37
ISR_NO_EC 38
ISR_NO_EC 39
ISR_NO_EC 40
ISR_NO_EC 41
ISR_NO_EC 42
ISR_NO_EC 43
ISR_NO_EC 44
ISR_NO_EC 45
ISR_NO_EC 46
ISR_NO_EC 47
