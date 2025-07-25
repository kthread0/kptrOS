#include "../include/system.h"
#include "cpu/access.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/pic.h"
#include "gpu/fb/fb.h"
#include "mem/paging/paging.h"
#include <stdbool.h>
#include <stdint.h>

#define LIMINE_REQUESTS_START_MARKER                                           \
	uint64_t limine_requests_start_marker[4] = {                               \
		0xf6b8f4b39de7d1ae, 0xfab91a6940fcb9cf, 0x785c6ed015d3e316,            \
		0x181e920a7852b9d9};

#define LIMINE_REQUESTS_END_MARKER                                             \
	uint64_t limine_requests_end_marker[2] = {0xadc0e0531bb10d03,              \
											  0x9572709f31764c62};

static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

extern void panic(cpu_state_t *state);

void kmain(void)
{
	if (memmap_request.response == NULL)
	{
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	debug_limine_requests();
	uint64_t limine_base_revision[3];
	// Ensure the bootloader actually understands our base revision (see spec).
	if (limine_base_revision[2] != 0)
	{
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	gdt_init();
	serial_write("GDT Initialized!\n");
	idt_init();
	serial_write("IDT Initialized!\n");

	fb_init();

	loadPageDirectory(page_directory);
	enablePaging();

	serial_write("Started!\n");
}
