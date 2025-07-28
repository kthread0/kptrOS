#include "gdt.h"

#include <stdint.h>
#include <system.h>

typedef struct {
	uint64_t null;				// 0x0000000000000000  NULL
	uint64_t kernel_code; // 0x00CF9A000000FFFF Kernel CODE
	uint64_t kernel_data; // 0x00CF92000000FFFF Kernel DATA
	uint64_t user_code;		// 0x00CFFA000000FFFF User CODE
	uint64_t user_data;		// 0x00CFF2000000FFFF User DATA
	uint64_t tss;					// 0x00FFE9000000FFFF Task Stack Segment
} __attribute__((packed)) gdt_t;

gdt_t gdt;

void reloadSegments();

void gdt_init(void) {
	gdt.null = 0x0000000000000000;
	gdt.kernel_code = 0x00CF92000000FFFF;
	gdt.kernel_data = 0x00CF9A000000FFFF;
	gdt.user_code = 0x00CFF2000000FFFF;
	gdt.user_data = 0x00CFF2000000FFFF;
	gdt.tss = 0x00FFE9000000FFFF;
	reloadSegments();
	serial_printf("garmin, initialize gdt!\n");
}
