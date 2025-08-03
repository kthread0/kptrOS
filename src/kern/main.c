#include "cpu/cpuid.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "mem/pmm.h"

#include <limine.h>
#include <system.h>

struct limine_memmap_request memmap = {.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

void kmain(void) {
	gdt_init();
	idt_init();
	pmm_init(memmap.response->entries);

	for (;;) {
	}
}
