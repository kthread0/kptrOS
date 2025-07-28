#include "cpu/clk/timer.h"
#include "cpu/cpuid.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "gpu/fb/framebuffer.h"
#include "kbd/keyboard.h"
#include "mem/pmm.h"

#include <limine.h>
#include <system.h>

// Define the memory map request
volatile struct limine_memmap_request memmap_request = { .id = LIMINE_MEMMAP_REQUEST, .revision = 0 };

void kmain(void) {
	gdt_init();
	idt_init();
	keyboard_init();
	if (check_fb() == true) {
		fbinit();
	}

	if (memmap_request.response == NULL) {
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}

	pmm_init(memmap_request.response);

	for (;;) {
		keyboard_interrupt_handler();
	}
}
