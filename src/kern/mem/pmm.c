#include "pmm.h"

#include <system.h> // For panic()

extern char _kernel_start[], _kernel_end[];

struct page {
	uint8_t in_use;
};

static struct page *page_metadata = NULL;
static uint64_t total_pages = 0;

void pmm_init(struct limine_memmap_response *memmap) {
	// Add null check for memmap
	if (memmap == NULL) {
		serial_printf("memmap is NULL!\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	uint64_t highest_addr = 0;
	uint64_t metadata_base = 0;

	// First, find the highest address to determine memory size
	for (uint64_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry *entry = memmap->entries[i];
		if (entry == NULL)
			continue;

		if (entry->type == LIMINE_MEMMAP_USABLE) {
			uint64_t top = entry->base + entry->length;
			if (top > highest_addr) {
				highest_addr = top;
			}
		}
	}

	// Add sanity check for highest_addr
	if (highest_addr == 0) {
		serial_printf("highest_addr is NULL!\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	total_pages = highest_addr / PAGE_SIZE;
	uint64_t metadata_size = total_pages * sizeof(struct page);

	// Second, find a place for our metadata
	for (uint64_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry *entry = memmap->entries[i];
		if (entry == NULL)
			continue;

		if (entry->type == LIMINE_MEMMAP_USABLE && entry->length >= metadata_size) {
			metadata_base = entry->base;
			break;
		}
	}

	if (metadata_base == 0) {
		serial_printf("metadata_base is NULL!\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	// If using virtual memory, ensure this address is mapped
	page_metadata = (struct page *) metadata_base;

	// Third, mark all pages as used initially
	for (uint64_t i = 0; i < total_pages; i++) {
		page_metadata[i].in_use = 1;
	}

	// Fourth, free all usable memory regions
	for (uint64_t i = 0; i < memmap->entry_count; i++) {
		struct limine_memmap_entry *entry = memmap->entries[i];
		if (entry == NULL || entry->type != LIMINE_MEMMAP_USABLE) {
			continue;
		}

		for (uint64_t j = 0; j < entry->length; j += PAGE_SIZE) {
			pmm_free_page((void *) (entry->base + j));
		}
	}

	// Fifth, re-mark the kernel and PMM metadata as used
	uint64_t kernel_start_page = (uint64_t) _kernel_start / PAGE_SIZE;
	uint64_t kernel_end_page = ((uint64_t) _kernel_end + PAGE_SIZE - 1) / PAGE_SIZE;
	for (uint64_t i = kernel_start_page; i < kernel_end_page; i++) {
		if (i < total_pages)
			page_metadata[i].in_use = 1;
	}

	uint64_t metadata_start_page = metadata_base / PAGE_SIZE;
	uint64_t metadata_end_page = (metadata_base + metadata_size + PAGE_SIZE - 1) / PAGE_SIZE;
	for (uint64_t i = metadata_start_page; i < metadata_end_page; i++) {
		if (i < total_pages)
			page_metadata[i].in_use = 1;
	}
}

void *pmm_alloc(size_t len) {
	size_t num_pages = 1 << len;
	uint64_t consecutive_pages = 0;

	for (uint64_t i = 0; i < total_pages; i++) {
		if (!page_metadata[i].in_use) {
			consecutive_pages++;
		} else {
			consecutive_pages = 0;
		}

		if (consecutive_pages >= num_pages) {
			uint64_t start_page_index = i - num_pages + 1;
			for (uint64_t j = 0; j < num_pages; j++) {
				page_metadata[start_page_index + j].in_use = 1;
			}
			return (void *) (start_page_index * PAGE_SIZE);
		}
	}

	return NULL; // Out of memory
}

void pmm_free(void *addr, size_t len) {
	if (addr == NULL)
		return;

	size_t num_pages = 1 << len;
	uint64_t start_page_index = (uint64_t) addr / PAGE_SIZE;

	for (uint64_t i = 0; i < num_pages; i++) {
		if (start_page_index + i < total_pages) {
			page_metadata[start_page_index + i].in_use = 0;
		}
	}
}

void *pmm_alloc_page() {
	return pmm_alloc(0);
}

void pmm_free_page(void *addr) {
	pmm_free(addr, 0);
}
