#include "paging.h"
#include "../serial/serial.h"
#include "kern/mem/pmm.h"
#include <limine.h>
#include <stdint.h>

#define HH_BASE 0xFFFFFFFF80000000
#define PAGE_SIZE 4096

volatile struct limine_executable_file_request executable_file_request = {
		.id = LIMINE_EXECUTABLE_FILE_REQUEST,
		.revision = 0};

uint64_t pml4[512] __attribute__((aligned(4096)));
uint64_t pdpt[512] __attribute__((aligned(4096)));
uint64_t pd[512] __attribute__((aligned(4096)));
uint64_t pt[512] __attribute__((aligned(4096)));

void load_pages(void) {
	int i = 0;
	for (uint64_t addr = 0; addr < 0x1000000; addr++) {
		pt[addr / PAGE_SIZE] = addr | 0x03;
	}
	pd[0] = (uint64_t)pt | 0x03;
	pdpt[0] = (uint64_t)pd | 0x03;
	pml4[0] = (uint64_t)pdpt | 0x03;

	uint64_t *kernel_addr = executable_file_request.response->executable_file->address;
	uint64_t kernel_size = executable_file_request.response->executable_file->size;

	for (uint64_t off = 0; off < kernel_size; off++) {
		uint64_t virt = HH_BASE + off;
		uint64_t phys = (uintptr_t)kernel_addr + off;
		size_t idx = (virt & 0x1FFFFF) / PAGE_SIZE;
		pt[idx] = phys | 0x03;
		i++;
	}

	serial_printf("%d pages loaded\n", i);
}
