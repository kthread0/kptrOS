%macro isr_err_stub 1

isr_stub_%+%1:
	push   rax
	push   rbx
	push   rcx
	push   rdx
	push   rsi
	push   rdi
	push   rbp
	push   r8
	push   r9
	push   r10
	push   r11
	push   r12
	push   r13
	push   r14
	push   r15
	pushfq ; Save flags
	mov    rdi, %1; Pass interrupt vector as first argument
	mov    rsi, [rsp + 8]; Pass error code as second argument
	call   exception_handler
	add    rsp, 8; Remove error code from stack
	popfq  ; Restore flags
	pop    r15
	pop    r14
	pop    r13
	pop    r12
	pop    r11
	pop    r10
	pop    r9
	pop    r8
	pop    rbp
	pop    rdi
	pop    rsi
	pop    rdx
	pop    rcx
	pop    rbx
	pop    rax
	iretq
	%endmacro

	%macro isr_no_err_stub 1

isr_stub_%+%1:
	push   rax
	push   rbx
	push   rcx
	push   rdx
	push   rsi
	push   rdi
	push   rbp
	push   r8
	push   r9
	push   r10
	push   r11
	push   r12
	push   r13
	push   r14
	push   r15
	pushfq ; Save flags
	mov    rdi, %1; Pass interrupt vector as first argument
	xor    rsi, rsi; Pass 0 as second argument (no error code)
	call   exception_handler
	popfq  ; Restore flags
	pop    r15
	pop    r14
	pop    r13
	pop    r12
	pop    r11
	pop    r10
	pop    r9
	pop    r8
	pop    rbp
	pop    rdi
	pop    rsi
	pop    rdx
	pop    rcx
	pop    rbx
	pop    rax
	iretq
	%endmacro

