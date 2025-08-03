global EnableSSE
global EnableFPU
global EnableXSAVE

EnableFPU:
	mov rax, cr0
	bts rax, 1
	btr rax, 2
	bts rax, 5
	btr rax, 3
	mov cr0, rax

	finit
	ret

EnableXSAVE:
	mov rax, cr4
	bts rax, 18
	mov cr4, rax
	mov rcx, 0
	xgetbv
	or  eax, 6
	mov rcx, 0
	xsetbv

	ret

EnableSSE:
	mov rax, cr4
	bts rax, 9
	bts rax, 10
	mov cr4, rax

	ret
