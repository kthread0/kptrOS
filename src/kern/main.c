#include "../include/limine.h"
#include "../include/system.h"
#include "cpu/access.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/pic.h"
#include "gpu/fb/fb.h"
#include "mem/paging/paging.h"
#include <stdbool.h>
#include <stdint.h>

extern void panic(cpu_state_t *state);

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((
	used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3)

	// The Limine requests can be placed anywhere, but it is important that
	// the compiler does not optimise them away, so, usually, they should
	// be made volatile or equivalent, _and_ they should be accessed at least
	// once or marked as used with the "used" attribute as done here.

	__attribute__((
		used, section(".limine_requests_"
					  "start"))) static volatile LIMINE_REQUESTS_START_MARKER

	__attribute__((
		used,
		section(
			".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER

	// The following will be our kernel's entry point.
	// If renaming kmain() to something else, make sure to change the
	// linker script accordingly.
	void kmain(void) {
	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	gdt_init();
	serial_write("GDT Initialized!\n");
	idt_init();
	serial_write("IDT Initialized!\n");

	fb_init();
	fb_draw();

	loadPageDirectory(page_directory);
	enablePaging();

	serial_write("Started!\n");
	cpu_state_t state;
	capture_cpu_state(&state);
	panic(&state);
}
