global userspace_jump
extern test_user_function
userspace_jump:
	push rax
	mov rax, (4 * 8) | 3 ; user data segment with RPL 3
	mov ds, rax
	mov es, rax
	mov fs, rax
	mov gs, rax ; sysexit sets SS

	xor rdx, rdx ; not necessary; set to 0
	mov rax, 0x8 ; the segments are computed as follows: CS=MSR+0x10 (0x8+0x10=0x18), SS=MSR+0x18 (0x8+0x18=0x20).
	mov rcx, 0x174 ; MSR specifier: IA32_SYSENTER_CS
	wrmsr ; set sysexit segments

	mov rdx, test_user_function ; to be loaded into EIP
	mov rcx, es
