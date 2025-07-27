#include "paging.h"
#include <stdint.h>

uint64_t page_dir_ptr_tab[4] __attribute__((aligned(0x20)));
uint64_t page_directory[512] __attribute__((aligned(0x1000)));
uint64_t page_table[512] __attribute__((aligned(0x1000)));
uint64_t pml4_table[512] __attribute__((aligned(0x1000)));

void load_page(unsigned int*) {
	// Initialize PML4 table
	for (int i = 0; i < 512; i++) {
		pml4_table[i] = 0x00000002;	 // Not present initially
	}

	pml4_table[0] = (uint64_t)&page_dir_ptr_tab | 0x03;	 // Present + Write

	page_dir_ptr_tab[0] = (uint64_t)&page_directory | 0x03;
	page_directory[0] = 0b10000011;

	asm volatile("mov %%cr4, %%rax; bts $5, %%rax; mov %%rax, %%cr4" ::
									 : "rax");												 // PAE
	asm volatile("mov %0, %%cr3" ::"r"(&pml4_table));	 // Load PML4 into CR3
	asm volatile("mov %%cr0, %%rax; or $0x1000, %%rax; mov %%rax, %%cr0;" ::
									 : "rax");	// Enable paging + long mode
}
