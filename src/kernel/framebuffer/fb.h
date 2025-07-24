#ifndef FB_H
#define FB_H

#include "../../include/system.h"

// Framebuffer information
struct framebuffer {
		uint32_t *address;
		uint32_t width;
		uint32_t height;
		uint32_t pitch;
		uint16_t bpp;
};

extern struct framebuffer fb;

void fb_init(void);
void fb_put_pixel(uint32_t, uint32_t, uint32_t);

#endif // FB_H
