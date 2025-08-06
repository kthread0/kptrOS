#ifndef PMM_H
#define PMM_H

#include "paging.h"
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_PAGES 65536 * 8

extern struct limine_memmap_request memmap;
void bitmap_init();
void *get_block_addr(int block_index);
void free_page(void *addr, size_t len);
void alloc_page(void **addr, size_t len);

void loadPageDirectory(uint64_t pml4);
void enablePaging();

#endif // PMM_H
