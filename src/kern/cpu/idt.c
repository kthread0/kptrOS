#include "idt.h"
#include "../serial/serial.h"

#include "../cpu/access.h"

#include <limine.h>
#include <panic.h>
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
	uint64_t rsi, rdi, rbp;
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
	} else { // CPU Exception
		__asm__ volatile("cli");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
}

void exception_handler(uint64_t vector, uint64_t error_code) {
	__asm__ volatile("cli");
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

extern void *isr_stub_table[];
extern void page_fault_isr_stub(void);

void idt_init() {
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

	for (int i = 0; i < 48; ++i) {
		idt_set_descriptor(i, isr_stub_table[i], 0x8E);
		asm volatile("lidt %0" : : "m"(idtr));
	}

	serial_write("[ OK ] IDT\n");
}
