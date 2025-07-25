#include "cpu/access.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/pic.h"
#include "gpu/fb/fb.h"
#include "mem/paging/paging.h"
#include "mem/pmm.h"
#include <stdbool.h>
#include <stdint.h>
#include <system.h>

extern void panic(cpu_state_t *state);

void kmain(void)
{
	gdt_init();
	serial_write("GDT Initialized!\n");
	idt_init();
	serial_write("IDT Initialized!\n");

	debug_limine_requests();

	if (are_interrupts_enabled() == false)
	{
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
	else
	{
		serial_write("IDT Initialized!\n");
	}

	fb_init();

	serial_write("Started!\n");
	for (;;)
	{
	}
}
