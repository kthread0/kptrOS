#include "idt.h"
#include "../serial/serial.h"

#include "../cpu/access.h"
#include "../hpet.h"

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

__attribute__((aligned(KERNEL_CS))) static idt_entry_t idt[IDT_MAX_DESCRIPTORS]; // Create an array of IDT entries; aligned

typedef struct {
	uint64_t rdx, rcx, rbx, rax; // General-purpose registers
	uint64_t rsi, rdi, rbp, rsp; // Stack and pointer registers
	uint64_t r15, r14, r13, r12; // Additional general-purpose registers
	uint64_t r11, r10, r9, r8;	 // Additional general-purpose registers
	uint64_t int_no, err_code;	 // Interrupt number and error code
	uint64_t rip, cs, rflags;		 // Instruction pointer, code segment, and flags
	uint64_t user_rsp, ss;			 // User stack pointer and segment
} __attribute__((packed)) registers_t;

volatile uint64_t tick = 0;

void page_fault_handler(registers_t *regs) {
	uintptr_t faulting_address = 0;
	asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

	serial_printf("[ ERR ] Page Fault at address: %x, Error Code: %x\n", faulting_address, regs->err_code);

	if (regs->err_code & 1) {
		serial_write("[ INFO ] Page-level protection violation\n");
	} else {
		serial_write("[ INFO ] Non-present page\n");
	}

	if (regs->err_code & 2) {
		serial_write("[ INFO ] Write access\n");
	} else {
		serial_write("[ INFO ] Read access\n");
	}

	if (regs->err_code & 4) {
		serial_write("[ INFO ] Fault in user-mode\n");
	} else {
		serial_write("[ INFO ] Fault in kernel-mode\n");
	}

	if (regs->err_code & 8) {
		serial_write("[ INFO ] Fault caused by reserved bit set\n");
	}

	if (regs->err_code & 16) {
		serial_write("[ INFO ] Instruction fetch fault\n");
	}

	exception_handler(regs->int_no, regs->err_code);
}

void hpet_interrupt_handler(registers_t *regs) {
	if (!hpet_base) {
		serial_write("[ ERR ] HPET base address is NULL\n");
		return;
	}

	uint64_t *interrupt_status = (uint64_t *)(hpet_base + 0x20);
	*interrupt_status = 1;

	tick++;
	serial_printf("[ INFO ] HPET Interrupt: Tick %d\n", tick);
}

void interrupt_handler(registers_t *regs) {
	if (!regs) {
		serial_write("[ ERR ] interrupt_handler called with NULL regs\n");
		return;
	}

	// serial_printf("[ DEBUG ] Interrupt vector: %x, Error Code: %x\n", regs->int_no, regs->err_code);

	if (regs->int_no == 2) {
		page_fault_handler(regs);
	} else if (regs->int_no == HPET_IRQ_VECTOR) {
		hpet_interrupt_handler(regs);
	} else {
		serial_printf("[ INFO ] Other Interrupt: Vector %x\n", regs->int_no);
	}
}

void exception_handler(uint64_t vector, uint64_t error_code) {
	serial_printf("[ EXCEPTION ] Vector: %x, Error Code: %x\n", vector, error_code);

	cpu_state_t state;
	capture_cpu_state(&state);

	// Log CPU state for debugging
	serial_printf("[ DEBUG ] Captured CPU State: RIP=%x, RSP=%x, RBP=%x\n",
								state.rip, state.rsp, state.rbp);

	if (state.rip == 0 || state.rip == 1) {
		serial_printf("[ ERR ] Invalid RIP value: %x\n", state.rip);
		while (1) {
			asm volatile("hlt");
		}
	}

	panic(&state);
}

void idt_set_descriptor(uint64_t vector, void *isr, uint8_t flags) {
	if (!isr) {
		serial_printf("[ ERR ] Null ISR for vector: %x\n", vector);
		return;
	}
	serial_printf("[ DEBUG ] Setting IDT vector: %x, ISR: %x\n", vector, isr);

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
	}

	idt_set_descriptor(14, isr_stub_table[14], 0x8E);
	asm volatile("lidt %0" : : "m"(idtr));

	serial_write("[ OK ] IDT\n");
}
