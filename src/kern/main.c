#include "cpu/cpuid.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"

#include <limine.h>
#include <system.h>

void kmain(void) {
	gdt_init();
	idt_init();
	debug_limine_requests();

	for (;;) {
	}
}
