global reload_segments

reload_segments:
	mov rax, 0x10
	mov ds, rax
	mov es, rax
	mov fs, rax
	mov gs, rax
	mov ss, rax

	pop  rdi
	push 0x08
	push rdi
	lretq
