#include "gdt.h"
#include "../serial/serial.h"

#include <panic.h>
#include <stdint.h>
#include <system.h>

typedef struct {
	uint64_t null;
	uint64_t kernel_code; // Kernel CODE
	uint64_t kernel_data; // Kernel DATA
	uint64_t user_code;		// User CODE
	uint64_t user_data;		// User DATA
} __attribute__((packed)) gdt_t;

typedef struct {
	uint16_t limit; // Size of the GDT
	uint64_t base;	// Base address of the GDT
} __attribute__((packed)) gdt_descriptor_t;

uint64_t gdt_entries[6];

gdt_t gdt;

gdt_descriptor_t descriptor = {
		.limit = sizeof(gdt) - 1,
		.base = (uint64_t)&gdt,
};

extern void reload_segments(void);

void gdt_init(void) {
	gdt.null = 0;
	gdt_entries[0] = gdt.null;

	gdt.kernel_code = 0;
	gdt.kernel_code |= 0b1011 << 8;
	gdt.kernel_code |= 1 << 12;
	gdt.kernel_code |= 0 << 13;
	gdt.kernel_code |= 1 << 15;
	gdt.kernel_code |= 1 << 21;
	gdt_entries[1] = gdt.kernel_code << 32;

	gdt.kernel_data = 0;
	gdt.kernel_data |= 0b0011 << 8;
	gdt.kernel_data |= 1 << 12;
	gdt.kernel_data |= 0 << 13;
	gdt.kernel_data |= 1 << 15;
	gdt.kernel_data |= 1 << 21;
	gdt_entries[2] = gdt.kernel_data << 32;

	gdt.user_code = 0;
	gdt.user_code = gdt.kernel_code | (3 << 13);
	gdt_entries[3] = gdt.user_code;

	gdt.user_data = 0;
	gdt.user_data = gdt.kernel_data | (3 << 13);
	gdt_entries[4] = gdt.user_data;

	reload_segments();
	asm volatile("lgdt %0" : : "m"(descriptor));

	serial_write("[ OK ] GDT\n");
}
