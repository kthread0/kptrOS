with open("isr_stubs.asm", "w") as f:
    f.write("section .text\n")
    for i in range(32):
        f.write(f"global isr_stub_{i}\n")
        f.write(f"isr_stub_{i}:\n")
        f.write("    iret\n")
