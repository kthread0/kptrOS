#include "fb.h"
#include "../../include/limine.h"
#include "../serial.h"
#include <stdbool.h>

// Limine framebuffer request
struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

struct framebuffer fb;

extern unsigned char _binary_font_psf_start;
extern unsigned char _binary_font_psf_end;

void fb_init(void) {
	serial_write("Attempting to request framebuffer...\n");

	if (framebuffer_request.response == NULL) {
		serial_write("Framebuffer response is NULL!\n");
		while (1)
			;
	}

	if (framebuffer_request.response->framebuffer_count < 1) {
		serial_write("No framebuffers provided by Limine!\n");
		while (1)
			;
	}

	struct limine_framebuffer *fb =
		framebuffer_request.response->framebuffers[0];

	serial_write("Framebuffer properties:\n");
	serial_write("Address: ");
	serial_write_hex((uint64_t)fb->address);
	serial_write("\n");
	serial_write("Width: ");
	serial_write_dec(fb->width);
	serial_write("\n");
	serial_write("Height: ");
	serial_write_dec(fb->height);
	serial_write("\n");
	serial_write("Pitch: ");
	serial_write_dec(fb->pitch);
	serial_write("\n");

	fb->address = fb->address;
	fb->width = fb->width;
	fb->width = fb->height;
	fb->pitch = fb->pitch;

	serial_write("Framebuffer request succeeded!\n");
}

void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= fb.width || y >= fb.height) {
		return; // Out-of-bounds check
	}
	fb.address[y * (fb.pitch / sizeof(uint32_t)) + x] = color;
}
