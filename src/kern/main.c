#include "cpu/access.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/pic.h"
#include "mem/paging/paging.h"
#include <limine.h>
#include <stdbool.h>
#include <stdint.h>
#include <system.h>

extern void panic(cpu_state_t *state);

void kmain(void)
{

	gdt_init();
	serial_write("GDT Initialized!\n");
	idt_init();
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

	fbinit();
	serial_printf("Hello from Serial Console!\n");

	for (;;)
	{
	}
}
