#include "idt.h"
#include "../cpu/access.h"
#include "../cpu/pic.h"
#include "../kbd/keyboard.h"
#include <limine.h>
#include <stdbool.h>
#include <system.h>

#define KERNEL_CS 0x28
#define IDT_MAX_DESCRIPTORS 256

typedef struct
{
	uint16_t isr_low;	// The lower 16 bits of the ISR's address
	uint16_t kernel_cs; // The GDT segment selector that the CPU will load
						// into CS before calling the ISR
	uint8_t ist; // The IST in the TSS that the CPU will load into RSP; set
				 // to zero for now
	uint8_t attributes; // Type and attributes; see the IDT page
	uint16_t isr_mid;	// The higher 16 bits of the lower 32 bits of the
						// ISR's address
	uint32_t isr_high;	// The higher 32 bits of the ISR's address
	uint32_t reserved;	// Set to zero
} __attribute__((packed)) idt_entry_t;

typedef struct
{
	uint16_t limit;
	uint64_t base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

__attribute__((aligned(0x10))) static idt_entry_t
	idt[IDT_MAX_DESCRIPTORS]; // Create an array of IDT entries; aligned for
							  // performance

void exception_handler(uint64_t vector, uint64_t error_code)
{
	__asm__ volatile("cli");
	// Handle keyboard interrupt safely
	if (vector == 33)
	{								  // IRQ1
		uint8_t scancode = inb(0x60); // Read scancode
		serial_printf("Key: 0x%02X\n", scancode);
		outb(0x20, 0x20); // Send EOI to PIC
		__asm__ volatile("sti");
		return;
	}

	serial_printf("Exception: Vector=%d, Error Code=%x\n", vector, error_code);
	cpu_state_t state;
	capture_cpu_state(&state);
	panic(&state);
}

void idt_set_descriptor(uint64_t vector, void *isr, uint8_t flags)
{
	idt_entry_t *descriptor = &idt[vector];
	uintptr_t isr_addr = (uintptr_t)isr;

	descriptor->isr_low = isr_addr & 0xFFFF;
	descriptor->kernel_cs = KERNEL_CS; // pls make a constant for this somewhere
	descriptor->ist = 0;
	descriptor->attributes = flags;
	descriptor->isr_mid = (isr_addr >> 16) & 0xFFFF;
	descriptor->isr_high = (isr_addr >> 32) & 0xFFFFFFFF;
	descriptor->reserved = 0;
}

static bool vectors[IDT_MAX_DESCRIPTORS];

extern void keyboard_isr_stub(void);
void *isr_stub_table[IDT_MAX_DESCRIPTORS];

void keyboard_isr_wrapper(void)
{
	keyboard_interrupt_handler();
}

void idt_init()
{
	idtr.base = (uintptr_t)&idt[0];
	idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
	for (uint16_t vector = 0; vector < IDT_MAX_DESCRIPTORS; vector++)
	{
		idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
		vectors[vector] = true;
	}

	idt_set_descriptor(33, keyboard_isr_wrapper, 0x8E);

	__asm__ volatile("lidt %0" : : "m"(idtr)); // load the new IDT
	__asm__ volatile("sti");				   // set the interrupt flag
}
