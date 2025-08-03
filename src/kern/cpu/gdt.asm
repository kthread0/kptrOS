global reload_segments

reload_segments:
	push qword 0x08
	lea  rax, [rel .reload_cs]
	push rax
	retfq

.reload_cs:
	mov rax, 0x10
	mov ds, rax
	mov es, rax
	mov fs, rax
	mov gs, rax
	mov ss, rax
	ret
