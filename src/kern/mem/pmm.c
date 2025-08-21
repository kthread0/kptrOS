#include "pmm.h"
#include <panic.h>
#include <stddef.h>
#include <stdint.h>
#include <system.h>

static uint64_t bitmap[MAX_PAGES / 2];
static uint64_t page_addrs[MAX_PAGES];
static size_t total_pages = MAX_PAGES;

static void set_bit(size_t index) { bitmap[index / 8] |= (1 << (index % 8)); }
static void clear_bit(size_t index) { bitmap[index / 8] &= ~(1 << (index % 8)); }
static int test_bit(size_t index) { return (bitmap[index / 8] & (1 << (index % 8))) != 0; }

struct limine_memmap_request memmap = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

void alloc_page(void **addr, size_t len) {
	len = PAGE_SIZE;

	for (size_t index = 0; index < total_pages; index++) {
		if (!test_bit(index)) {
			set_bit(index);
			*addr = (void *)(uint64_t)page_addrs[index];
			return;
		}
	}
}

void free_page(void *addr, size_t len) {
	len = PAGE_SIZE;

	uint64_t paddr = (uint64_t)addr;

	for (size_t index = 0; index < total_pages; index++) {
		if (page_addrs[index] == paddr) {
			memset(addr, 0, PAGE_SIZE);
			clear_bit(index);
			return;
		}
	}
}

unsigned char memory_pool[PAGE_SIZE];

void *get_block_addr(int block_index) {
	if (block_index >= 0 && block_index < PAGE_SIZE) {
		return &memory_pool[block_index * PAGE_SIZE];
	}
	return NULL;
}

void bitmap_init() {
	total_pages = 0;

	for (size_t i = 0; i < memmap.response->entry_count; i++) {
		struct limine_memmap_entry *entry = memmap.response->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE) {
			uint64_t region_start = entry->base;
			uint64_t region_end = entry->base + entry->length;

			uint64_t page_start = (region_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
			uint64_t page_end = region_end & ~(PAGE_SIZE - 1);

			for (uint64_t addr = page_start; addr < page_end; addr += PAGE_SIZE) {
				if (total_pages < MAX_PAGES) {
					page_addrs[total_pages] = addr;
					clear_bit(total_pages);
					total_pages++;
				}
			}
		}
	}
	for (size_t i = 0; i < memmap.response->entry_count; i++) {
		struct limine_memmap_entry *entry = memmap.response->entries[i];
		if (entry->type != LIMINE_MEMMAP_USABLE) {
			uint64_t region_start = entry->base;
			uint64_t region_end = entry->base + entry->length;

			uint64_t page_start = (region_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
			uint64_t page_end = region_end & ~(PAGE_SIZE - 1);

			for (uint64_t addr = page_start; addr > page_end; addr -= PAGE_SIZE) {
				if (total_pages > MAX_PAGES) {
					page_addrs[total_pages] = addr;
					set_bit(total_pages);
					total_pages--;
				}
			}
		}
	}

	load_pages();
}
