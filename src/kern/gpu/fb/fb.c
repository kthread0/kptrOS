#include "../../../include/limine.h"
#include "../../../include/system.h"
#include "../../../kern/serial/serial.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// External framebuffer request from Limine
volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

// Function to check framebuffer availability
bool fb_avail() {
	return framebuffer_request.response != NULL &&
		   framebuffer_request.response->framebuffer_count > 0;
}

// Function to get the first framebuffer
struct limine_framebuffer *get_framebuffer() {
	if (!fb_avail()) {
		return NULL;
	}
	return framebuffer_request.response->framebuffers[0];
}

// Function to initialize the framebuffer (e.g., clear screen)
void fb_init() {
	if (framebuffer_request.response == NULL) {
		serial_write("Framebuffer request response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	if (framebuffer_request.response->framebuffer_count < 1) {
		serial_write("No framebuffers provided by bootloader.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	struct limine_framebuffer *framebuffer =
		framebuffer_request.response->framebuffers[0];
	serial_write("Framebuffer initialized.\n");
	serial_write("Width: ");
	serial_write_dec(framebuffer->width);
	serial_write("\nHeight: ");
	serial_write_dec(framebuffer->height);
	serial_write("\nPitch: ");
	serial_write_dec(framebuffer->pitch);
	serial_write("\n");

	// Clear the screen
	volatile uint32_t *fb_ptr = framebuffer->address;
	for (size_t i = 0; i < framebuffer->width * framebuffer->height; i++) {
		fb_ptr[i] = 0x000000; // Black color
	}
}

// Function to draw a yellow diagonal line (example)
void fb_draw() {
	if (!fb_avail()) {
		serial_write("Framebuffer not available!\n");
		return;
	}

	struct limine_framebuffer *framebuffer = get_framebuffer();

	// Draw a yellow diagonal line
	for (size_t i = 0; i < framebuffer->width && i < framebuffer->height; i++) {
		volatile uint32_t *fb_ptr = framebuffer->address;
		fb_ptr[i * framebuffer->pitch / 4 + i] = 0xFFFF00; // Yellow color
	}
}
