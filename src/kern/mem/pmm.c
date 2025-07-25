#include "pmm.h"
#include <stddef.h> // for NULL

extern char _kernel_start[], _kernel_end[];

struct free_list_node
{
	struct free_list_node *next;
};

struct page
{
	uint8_t in_use;
	uint8_t order;
};

static struct free_list_node *free_lists[MAX_ORDER];
static struct page *page_metadata;
static uint64_t total_pages;
static uint64_t highest_addr = 0;

static inline uint64_t get_buddy_addr(uint64_t addr, size_t order)
{
	return addr ^ (1UL << (order + 12));
}

static void pmm_free_early(void *addr, size_t order);

void pmm_init(struct limine_memmap_response *memmap)
{
	for (uint64_t i = 0; i < memmap->entry_count; i++)
	{
		struct limine_memmap_entry *entry = memmap->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE)
		{
			uint64_t top = entry->base + entry->length;
			if (top > highest_addr)
			{
				highest_addr = top;
			}
		}
	}
	total_pages = highest_addr / PAGE_SIZE;

	uint64_t metadata_size = total_pages * sizeof(struct page);
	struct limine_memmap_entry *metadata_region = NULL;
	for (uint64_t i = 0; i < memmap->entry_count; i++)
	{
		struct limine_memmap_entry *entry = memmap->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE &&
			entry->length >= metadata_size)
		{
			page_metadata = (struct page *)entry->base;
			metadata_region = entry;
			break;
		}
	}
	for (uint64_t i = 0; i < total_pages; i++)
	{
		page_metadata[i].in_use = 1;
		page_metadata[i].order = 0;
	}
	for (int i = 0; i < MAX_ORDER; i++)
	{
		free_lists[i] = NULL;
	}

	for (uint64_t i = 0; i < memmap->entry_count; i++)
	{
		struct limine_memmap_entry *entry = memmap->entries[i];
		if (entry->type == LIMINE_MEMMAP_USABLE)
		{
			// Free memory in page-sized chunks
			for (uint64_t j = 0; j < entry->length; j += PAGE_SIZE)
			{
				pmm_free_early((void *)(entry->base + j), 0);
			}
		}
	}

	uint64_t kernel_start = (uint64_t)_kernel_start;
	uint64_t kernel_end = (uint64_t)_kernel_end;
	for (uint64_t i = kernel_start; i < kernel_end; i += PAGE_SIZE)
	{
		pmm_alloc_page();
	}
	for (uint64_t i = (uint64_t)page_metadata;
		 i < (uint64_t)page_metadata + metadata_size; i += PAGE_SIZE)
	{
		pmm_alloc_page();
	}
}

void *pmm_alloc(size_t order)
{
	if (order >= MAX_ORDER)
		return NULL;

	size_t current_order;
	for (current_order = order; current_order < MAX_ORDER; ++current_order)
	{
		if (free_lists[current_order])
			break;
	}

	if (current_order == MAX_ORDER)
		return NULL;

	struct free_list_node *block = free_lists[current_order];
	free_lists[current_order] = block->next;

	while (current_order > order)
	{
		current_order--;
		uint64_t buddy_addr = (uint64_t)block + (1UL << (current_order + 12));

		struct free_list_node *buddy_node = (struct free_list_node *)buddy_addr;
		buddy_node->next = free_lists[current_order];
		free_lists[current_order] = buddy_node;
		page_metadata[buddy_addr / PAGE_SIZE].order = current_order;
		page_metadata[buddy_addr / PAGE_SIZE].in_use = 0;
	}

	uint64_t page_index = (uint64_t)block / PAGE_SIZE;
	page_metadata[page_index].in_use = 1;
	page_metadata[page_index].order = order;

	return block;
}

void pmm_free(void *addr, size_t order)
{
	uint64_t page_index = (uint64_t)addr / PAGE_SIZE;

	while (order < MAX_ORDER - 1)
	{
		uint64_t buddy_addr = get_buddy_addr((uint64_t)addr, order);
		uint64_t buddy_page_index = buddy_addr / PAGE_SIZE;

		if (buddy_page_index >= total_pages ||
			page_metadata[buddy_page_index].in_use ||
			page_metadata[buddy_page_index].order != order)
		{
			break;
		}

		struct free_list_node *list = free_lists[order];
		struct free_list_node *prev = NULL;
		while (list)
		{
			if (list == (struct free_list_node *)buddy_addr)
			{
				if (prev)
					prev->next = list->next;
				else
					free_lists[order] = list->next;
				break;
			}
			prev = list;
			list = list->next;
		}

		if (buddy_addr < (uint64_t)addr)
		{
			addr = (void *)buddy_addr;
		}
		order++;
	}

	struct free_list_node *node = (struct free_list_node *)addr;
	page_index = (uint64_t)addr / PAGE_SIZE;
	page_metadata[page_index].in_use = 0;
	page_metadata[page_index].order = order;
	node->next = free_lists[order];
	free_lists[order] = node;
}

static void pmm_free_early(void *addr, size_t order)
{
	pmm_free(addr, order);
}

void *pmm_alloc_page()
{
	return pmm_alloc(0);
}

void pmm_free_page(void *addr)
{
	if (!addr)
		return;
	uint64_t page_index = (uint64_t)addr / PAGE_SIZE;
	if (page_index >= total_pages)
		return;
	pmm_free(addr, page_metadata[page_index].order);
}
