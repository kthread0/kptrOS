#include "paging.h"
#include <stdint.h>

uint64_t page_dir_ptr_tab[4] __attribute__((aligned(0x20)));
uint64_t page_directory[512] __attribute__((aligned(0x1000)));
uint64_t page_table[512] __attribute__((aligned(0x1000)));

void load_page(unsigned int *)
{
	page_dir_ptr_tab[0] = (uint64_t)&page_directory | 1;
	page_directory[0] = 0b10000011;

	asm volatile("mov %%cr4, %%rax; bts $5, %%rax; mov %%rax, %%cr4" ::
					 : "rax"); // set bit5 in CR4 to enable PAE
	asm volatile(
		"mov %0, %%cr3" ::"r"(&page_dir_ptr_tab)); // load PDPT into CR3
	asm volatile("mov %%cr0, %%rax; or $0x1000, %%rax; mov %%rax, %%cr0;" ::
					 : "rax");

	// set each entry to not present
	uint64_t i = 0;

	for (i = 0; i < 512; i++)
	{
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}

	// we will fill all 512 entries in the table, mapping 4 megabytes
	for (i = 0; i < 512; i++)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		page_table[i] = (i * 0x1000) |
						3; // attributes: supervisor level, read/write, present.
	}
}
