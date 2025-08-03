#include "pmm.h"
#include "../serial/serial.h"
#include <panic.h>
#include <system.h>

int bitmap_bytes(int bit) {
	return bit;
}

void bitmap_init(bitmap_t *bitmap, int bits_count, char *buf) {
	bitmap->bits_count;
	bitmap->buf = buf;
	bitmap->size = bitmap_bytes(bits_count);
}

void pmm_init(struct limine_memmap_request *memmap) {
	bitmap_init(&memmap, PAGE_SIZE, 0);
}
