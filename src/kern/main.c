#include "../include/limine.h"
#include "../include/system.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "serial.h"
#include <stdbool.h>
#include <stdint.h>

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((
	used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((
	used,
	section(
		".limine_requests"))) static volatile struct limine_framebuffer_request
	framebuffer_request = {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
			   section(".limine_requests_"
					   "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
	used,
	section(
		".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

static inline void outb(uint16_t port, uint8_t value) {
	__asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

__attribute__((interrupt)) void exception_handler(void *stack_frame
												  __attribute__((unused))) {
	while (1)
		; // Hang the system to indicate an exception occurred
}

void idt_setup_debug(void) {
	for (int i = 0; i < 256; i++) {
		idt_set_entry(i, exception_handler, 0x08, 0x8E);
	}
	idt_load();
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		serial_write("Kernel panicked!\n");
		panic();
	}
	gdt_init();
	serial_write("GDT Initialized!\n");
	idt_init();
	serial_write("IDT Initialized!\n");

	// Ensure we got a framebuffer.
	if (framebuffer_request.response == NULL ||
		framebuffer_request.response->framebuffer_count < 1) {
		serial_write("Couldn't find a framebuffer!\n");
		panic();
	}

	// Fetch the first framebuffer.
	struct limine_framebuffer *framebuffer =
		framebuffer_request.response->framebuffers[0];

	// Note: we assume the framebuffer model is RGB with 32-bit pixels.
	for (size_t i = 0; i < 100; i++) {
		volatile uint32_t *fb_ptr = framebuffer->address;
		fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
	}

	serial_write("Started!\n");
	panic();
}
