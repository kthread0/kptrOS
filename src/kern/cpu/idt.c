#include "idt.h"

#include "../cpu/access.h"
#include "../cpu/pic.h"
#include "../kbd/keyboard.h"

#include <limine.h>
#include <stdbool.h>
#include <system.h>

typedef struct {
	uint16_t isr_low;		// The lower 16 bits of the ISR's address
	uint16_t kernel_cs; // The GDT segment selector that the CPU will load
											// into CS before calling the ISR
	uint8_t ist;				// The IST in the TSS that the CPU will load into RSP; set
											// to zero for now
	uint8_t attributes; // Type and attributes; see the IDT page
	uint16_t isr_mid;		// The higher 16 bits of the lower 32 bits of the
											// ISR's address
	uint32_t isr_high;	// The higher 32 bits of the ISR's address
	uint32_t reserved;	// Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

__attribute__((aligned(0x10))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS]; // Create an array of IDT entries; aligned
																																						// for performance

typedef struct {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rsi, rdi, rbp; // Correct order
	uint64_t rdx, rcx, rbx, rax;
	uint64_t int_no, err_code;
	uint64_t rip, cs, rflags, user_rsp, ss;
} __attribute__((packed)) registers_t;

volatile uint64_t tick = 0;

void interrupt_handler(registers_t *regs) {
	if (regs->int_no >= 32) {		// Hardware IRQ
		if (regs->int_no == 33) { // Keyboard
			uint8_t scancode = inb(0x60);
			serial_printf("[ksc: 0x%x]", scancode);
		}
		pic_send_eoi(regs->int_no - 32);
	} else { // CPU Exception
		serial_printf("\nCPU EXCEPTION %d, ERR_CODE %d at RIP=0x%lx\n", regs->int_no, regs->err_code, regs->rip);
		serial_write("SYSTEM HALTED\n");
		for (;;) {
			asm("cli; hlt");
		}
	}
}

// The page fault handler
void page_fault_handler(registers_t *regs) {
	uint64_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

	serial_write("\n--- KERNEL PAGE FAULT ---\n");
	serial_printf("Faulting Address: 0x%lx\n", faulting_address);
	serial_printf("Instruction Pointer (RIP): 0x%lx\n", regs->rip);
	serial_printf("Error Code: 0x%lx\n", regs->err_code);
	serial_printf("Stack Pointer (RSI): 0x%lx\n", regs->rsi);
	serial_printf("Flags (RFLAGS): 0x%lx\n", regs->rflags);
	serial_write("Error Code Details:\n");
	serial_printf("  - %s\n", (regs->err_code & 0x1) ? "Protection violation" : "Page not present");
	serial_printf("  - Caused by a %s\n", (regs->err_code & 0x2) ? "write operation" : "read operation");
	serial_printf("  - Processor was in %s\n", (regs->err_code & 0x4) ? "user-mode" : "kernel-mode");
	if (regs->err_code & 0x8)
		serial_write("  - Reserved bits in page entry were set\n");
	if (regs->err_code & 0x10)
		serial_write("  - Caused by an instruction fetch\n");

	serial_write("System halted.\n");
	for (;;)
		; // Halt
}

void exception_handler(uint64_t vector, uint64_t error_code) {
	__asm__ volatile("cli");
	serial_printf("Exception: Vector=%d, Error Code=%x\n", vector, error_code);
	cpu_state_t state;
	capture_cpu_state(&state);
	panic(&state);
}

void idt_set_descriptor(uint64_t vector, void *isr, uint8_t flags) {
	idt_entry_t *descriptor = &idt[vector];
	uintptr_t isr_addr = (uintptr_t)isr;

	descriptor->isr_low = isr_addr & 0xFFFF;
	descriptor->kernel_cs = KERNEL_CS;
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = (isr_addr >> 16) & 0xFFFF;
	descriptor->isr_high = (isr_addr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

extern void keyboard_isr_stub(void);
extern void *isr_stub_table[];
extern void page_fault_isr_stub(void);

void keyboard_isr_wrapper(void) {
	keyboard_interrupt_handler();
}

void idt_init() {
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (int i = 0; i < 48; ++i) {
		idt_set_descriptor(i, isr_stub_table[i], 0x8E);
	}

	asm volatile("lidt %0" : : "m"(idtr));
}
