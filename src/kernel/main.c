#include "../include/limine.h"
#include "../include/system.h"
#include "framebuffer/fb.h"
#include "framebuffer/font.h"
#include "idt.h"
#include "serial.h"
#include <stdbool.h>
#include <stdint.h>

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((
	used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used,
			   section(".limine_requests_"
					   "start"))) static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((
	used,
	section(
		".limine_requests_end"))) static volatile LIMINE_REQUESTS_END_MARKER;

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .c file.

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
	__asm__ volatile("cli");
	idt_init();
	serial_write("IDT Initialized!\n");

	fb_init();
	// Render some text
	font_text("Hello, kptrOS!", 50, 50, 0xFFFFFF); // White color
	serial_write("Framebuffer renderer!\n");
	while (1)
		;
}
