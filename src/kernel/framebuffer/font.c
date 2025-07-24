#include "fb.h"
#include <stddef.h>
#include <stdint.h>

// PSF font header structure
struct psf2_header {
		uint32_t magic;
		uint32_t version;
		uint32_t headersize;
		uint32_t flags;
		uint32_t length;
		uint32_t charsize;
		uint32_t height;
		uint32_t width;
};

// Global variables for font
static struct psf2_header *font_header;
static void *font_glyphs;

void font_init(void *font_address) {
	font_header = (struct psf2_header *)font_address;
	font_glyphs = (void *)((uint8_t *)font_address + font_header->headersize);
}

void font_render(char c, uint32_t x, uint32_t y, uint32_t color) {
	if (!font_header) {
		return;
	}

	uint8_t *glyph = (uint8_t *)font_glyphs + (c * font_header->charsize);
	for (uint32_t row = 0; row < font_header->height; row++) {
		for (uint32_t col = 0; col < font_header->width; col++) {
			if (glyph[row] & (0x80 >> col)) {
				fb_put_pixel(x + col, y + row, color);
			}
		}
	}
}

void font_text(const char *text, uint32_t x, uint32_t y, uint32_t color) {
	uint32_t cursor_x = x;
	while (*text) {
		font_render(*text, cursor_x, y, color);
		cursor_x += font_header->width;
		text++;
	}
}
