extern reload_segments

reload_segments:
	PUSH 0x08; Push code segment to stack
	LEA  RAX, [rel .reload_cs]; Load address of .reload_CS into RAX
	PUSH RAX
	RETFQ

.reload_cs:
	MOV RAX, 0x10
	MOV DS, RAX
	MOV ES, RAX
	MOV FS, RAX
	MOV GS, RAX
	MOV SS, RAX
	RET
