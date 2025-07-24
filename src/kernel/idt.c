#include "idt.h"

// Define the IDT with 256 entries (maximum for x86_64)
#define IDT_ENTRIES 256
static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_descriptor;

// Default empty interrupt handler
__attribute__((interrupt)) void default_interrupt_handler(void *stack_frame) {
	// Hang the system to indicate an interrupt occurred
	while (1)
		;
}

// Set an entry in the IDT
void idt_set_entry(int vector, void (*handler)(), uint16_t selector,
				   uint8_t type_attr) {
	uint64_t handler_address = (uint64_t)handler;
	idt[vector].offset_low = handler_address & 0xFFFF;
	idt[vector].selector = selector;
	idt[vector].ist = 0; // No IST used here
	idt[vector].type_attr = type_attr;
	idt[vector].offset_mid = (handler_address >> 16) & 0xFFFF;
	idt[vector].offset_high = (handler_address >> 32);
	idt[vector].zero = 0;
}

// Load the IDT
void idt_load(void) {
	idt_descriptor.limit = sizeof(idt) - 1;
	idt_descriptor.base = (uint64_t)&idt;

	// Load the IDT using the `lidt` instruction
	__asm__ volatile("lidt %0" : : "m"(idt_descriptor));
}

// Initialize the IDT
void idt_init(void) {
	// Clear the IDT
	for (int i = 0; i < IDT_ENTRIES; i++) {
		idt_set_entry(i, default_interrupt_handler, 0x08, 0x8E);
	}

	// Load the IDT
	idt_load();
}
