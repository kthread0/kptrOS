#ifndef FB_H
#define FB_H

#include "../../../include/limine.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Function declarations
bool fb_avail();
struct limine_framebuffer *get_framebuffer();
void fb_init();
void fb_draw();

#endif // FB_H
