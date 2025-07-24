#include "fb.h"
#include "../../include/limine.h"
#include <stdbool.h>

// Limine framebuffer request
struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

// Framebuffer information
struct framebuffer {
		uint32_t *address;
		uint32_t width;
		uint32_t height;
		uint32_t pitch;
		uint16_t bpp;
} fb;

extern unsigned char _binary_font_psf_start;
extern unsigned char _binary_font_psf_end;

void fb_init(void) {
	if (framebuffer_request.response == NULL ||
		framebuffer_request.response->framebuffer_count < 1) {
		// Handle error: No framebuffer available
		return;
	}

	struct limine_framebuffer *framebuffer =
		framebuffer_request.response->framebuffers[0];
	fb.address = (uint32_t *)framebuffer->address;
	fb.width = framebuffer->width;
	fb.height = framebuffer->height;
	fb.pitch = framebuffer->pitch;
	fb.bpp = framebuffer->bpp;
}

void fb_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x >= fb.width || y >= fb.height) {
		return; // Out-of-bounds check
	}
	fb.address[y * (fb.pitch / sizeof(uint32_t)) + x] = color;
}
