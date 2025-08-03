global reload_segments

reload_segments:
	mov rax, 0x10
	mov ds, rax
	mov es, rax
	mov ss, rax
	mov fs, rax
	mov gs, rax

	push qword 0x08
	lea  rax, [rel .reload_cs]
	push rax
	retfq

.reload_cs:
	ret
