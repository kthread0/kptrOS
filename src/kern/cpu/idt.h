#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT Entry structure
struct idt_entry {
		uint16_t offset_low;  // Lower 16 bits of handler function address
		uint16_t selector;	  // Code segment selector in GDT
		uint8_t ist;		  // Interrupt Stack Table (IST) offset
		uint8_t type_attr;	  // Type and attributes
		uint16_t offset_mid;  // Middle 16 bits of handler function address
		uint32_t offset_high; // Higher 32 bits of handler function address
		uint32_t zero;		  // Reserved
} __attribute__((packed));

// IDT Pointer structure
struct idt_ptr {
		uint16_t limit; // Size of the IDT (entries * sizeof(idt_entry) - 1)
		uint64_t base;	// Address of the first IDT entry
} __attribute__((packed));

// Function declarations
void idt_set_entry(int vector, void (*handler)(), uint16_t selector,
				   uint8_t type_attr);
void idt_load(void);
void idt_init(void);

#endif // IDT_H
