#ifndef PMM_H
#define PMM_H

#include <limine.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
	char *buf;
	uint64_t size;
	uint8_t bits_count;
} bitmap_t;

int bitmap_bytes(int bit);

void bitmap_init(bitmap_t *bitmap, int bits_count, char *buf);
void bitmap_bit(bitmap_t *bitmap, uint8_t bit, int status);

extern struct limine_memmap_request memmap;
void pmm_init(struct limine_memmap_request *memmap);

#endif // PMM_H
