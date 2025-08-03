#ifndef PMM_H
#define PMM_H

#include "paging.h"
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

extern struct limine_memmap_request memmap;
void bitmap_init(struct limine_memmap_request *memmap);
void *get_block_addr(int block_index);
void free_page(void *addr);
void *alloc_page(void);

#endif // PMM_H
