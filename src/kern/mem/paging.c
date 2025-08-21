#include "paging.h"
#include "../serial/serial.h"
#include "kern/mem/pmm.h"
#include <limine.h>
#include <stdint.h>
#include <system.h>

volatile struct limine_executable_file_request executable_file_request = {
		.id = LIMINE_EXECUTABLE_FILE_REQUEST,
		.revision = 0};

volatile struct limine_hhdm_request hhdm_request = {.id = LIMINE_HHDM_REQUEST, .revision = 0};

uint64_t pml4[ENTRIES] __attribute__((aligned(PAGE_SIZE)));
uint64_t pdpt[ENTRIES] __attribute__((aligned(PAGE_SIZE)));
uint64_t pd[ENTRIES] __attribute__((aligned(PAGE_SIZE)));
uint64_t pt[ENTRIES] __attribute__((aligned(PAGE_SIZE)));

void load_pages(void) {
	uint64_t i = 0;
	for (uint64_t addr = 0; addr < MAX_PAGES; addr++) {
		pt[addr / PAGE_SIZE] = addr | 0x03;
	}
	pd[0] = (uint64_t)pt | 0x03;
	pdpt[0] = (uint64_t)pd | 0x03;
	pml4[0] = (uint64_t)pdpt | 0x03;

	uint64_t *kernel_addr = executable_file_request.response->executable_file->address;

	for (uint64_t off = 0; off < MAX_PAGES; off++) {
		uint64_t virt = hhdm_request.response->offset + off;
		uint64_t phys = (uintptr_t)kernel_addr + off;
		size_t index = (virt & 0x1FFFFF) / PAGE_SIZE;
		pt[index] = phys | 0x03;
		for (int i = 0; i < MAX_PAGES; i++) {
			*pt = virt | 1; // mark page present.
		}
		i++;
	}

	serial_printf("[ INFO ] %d Pages allocated\n", i);
}
