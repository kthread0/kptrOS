#include "gdt.h"

#include <stdint.h>
#include <system.h>

typedef struct {
	uint64_t null;				// 0x0000000000000000  NULL
	uint64_t kernel_code; // 0x00CF9A000000FFFF Kernel CODE
	uint64_t kernel_data; // 0x00CF92000000FFFF Kernel DATA
	uint64_t user_code;		// 0x00CFFA000000FFFF User CODE
	uint64_t user_data;		// 0x00CFF2000000FFFF User DATA
												// uint64_t tss;					// 0x00FFE9000000FFFF Task Stack Segment
} __attribute__((packed)) gdt_t;

typedef struct {
	uint16_t limit; // Size of the GDT
	uint64_t base;	// Base address of the GDT
} __attribute__((packed)) gdt_descriptor_t;

gdt_t gdt;

void gdt_init(void) {
	gdt.null = 0x0000000000000000;
	gdt.kernel_code = 0x00CF92000000FFFF;
	gdt.kernel_data = 0x00CF9A000000FFFF;
	gdt.user_code = 0x00CFF2000000FFFF;
	gdt.user_data = 0x00CFF2000000FFFF;
	// gdt.tss = 0x00FFE9000000FFFF;

	gdt_descriptor_t descriptor = {
		.limit = sizeof(gdt) - 1,
		.base = (uint64_t) &gdt,
	};

	__asm__ volatile("lgdt %0" : : "m"(descriptor));
	reload_segments();

	serial_printf("garmin, initialize gdt!\n");
}
