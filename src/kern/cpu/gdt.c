#include "gdt.h"
#include "../serial/serial.h"

#include <panic.h>
#include <stdint.h>
#include <system.h>

#define KCS 0x00AF9A000000FFFF
#define KDS 0x00CF92000000FFFF
#define UCS 0x00CFFA000000FFFF
#define UDS 0x00CFF2000000FFFF
#define TSS 0x00FFE9000000FFFF

typedef struct {
	uint64_t null;
	uint64_t kernel_code; // 0x00CF9A000000FFFF Kernel CODE
	uint64_t kernel_data; // 0x00CF92000000FFFF Kernel DATA
	uint64_t user_code;		// 0x00CFFA000000FFFF User CODE
	uint64_t user_data;		// 0x00CFF2000000FFFF User DATA
	uint64_t tss;					// 0x00FFE9000000FFFF Task Stack Segment
} __attribute__((packed)) gdt_t;

typedef struct {
	uint16_t limit; // Size of the GDT
	uint64_t base;	// Base address of the GDT
} __attribute__((packed)) gdt_descriptor_t;

gdt_t gdt;

gdt_descriptor_t descriptor = {
		.limit = sizeof(gdt) - 1,
		.base = (uint64_t)&gdt,
};

extern void reload_segments(void);

void gdt_init(void) {
	gdt.null = 0;
	gdt.kernel_code = KCS;
	gdt.kernel_data = KDS;
	gdt.user_code = UCS;
	gdt.user_data = UDS;
	gdt.tss = TSS;

	asm volatile("lgdt %0" : : "m"(descriptor));

	reload_segments();

	serial_write("[ OK ] GDT\n");
}
