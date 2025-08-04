#include "pmm.h"
#include "../serial/serial.h"
#include <panic.h>
#include <stddef.h>
#include <stdint.h>
#include <system.h>

#define MAX_PAGES 65536

static uint64_t bitmap[MAX_PAGES / 8];
static uintptr_t page_addrs[MAX_PAGES];
static size_t total_pages = 0;

static void set_bit(size_t rdx) { bitmap[rdx / 8] |= (1 << (rdx % 8)); }
static void clear_bit(size_t rdx) { bitmap[rdx / 8] &= ~(1 << (rdx % 8)); }
static int test_bit(size_t rdx) { return (bitmap[rdx / 8] & (1 << (rdx % 8))) != 0; }

struct limine_memmap_request memmap = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

void alloc_page(void **addr, size_t len) {
	len = PAGE_SIZE;

	if (len % PAGE_SIZE != 0) {
		serial_write("[ ERR ] Requested length is not page-aligned\n");
		*addr = NULL;
		return;
	}

	for (size_t rdx = 0; rdx < total_pages; rdx++) {
		if (!test_bit(rdx)) {
			set_bit(rdx);
			*addr = (void *)(uintptr_t)page_addrs[rdx];
			return;
		}
	}

	serial_write("[ ERR ] Not enough memory\n");
	*addr = NULL;
}

void free_page(void *addr, size_t len) {
	len = PAGE_SIZE;

	uintptr_t paddr = (uintptr_t)addr;

	if (paddr % PAGE_SIZE != 0) {
		serial_printf("[ ERR ] Unaligned physical address in free_page: paddr=%x\n", paddr);
		return;
	}

	for (size_t rdx = 0; rdx < total_pages; rdx++) {
		if (page_addrs[rdx] == paddr) {
			clear_bit(rdx);
			return;
		}
	}

	serial_printf("[ ERR ] Physical address not found in page_addrs: paddr=%x\n", paddr);
}

unsigned char memory_pool[PAGE_SIZE * 2];

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
			uintptr_t region_start = entry->base;
			uintptr_t region_end = entry->base + entry->length;

			uintptr_t page_start = (region_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
			uintptr_t page_end = region_end & ~(PAGE_SIZE - 1);

			for (uintptr_t addr = page_start; addr < page_end; addr += PAGE_SIZE) {
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
			uintptr_t region_start = entry->base;
			uintptr_t region_end = entry->base + entry->length;

			uintptr_t page_start = (region_start + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
			uintptr_t page_end = region_end & ~(PAGE_SIZE - 1);

			for (size_t rdx = 0; rdx < total_pages; rdx++) {
				uintptr_t paddr = page_addrs[rdx];
				if (paddr >= page_start && paddr < page_end) {
					set_bit(rdx);
				}
			}
		}
	}
}
