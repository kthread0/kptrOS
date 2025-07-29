#include "cpu/clk/timer.h"
#include "cpu/cpuid.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/userspace.h"
#include "kbd/keyboard.h"

#include <limine.h>
#include <system.h>

void kmain(void) {
	gdt_init();
	idt_init();
	debug_limine_requests();
	keyboard_init();
	fbinit();

	for (;;) {
		keyboard_interrupt_handler();
	}
}
