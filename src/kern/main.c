#include "acpi/init.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "kbd/keyboard.h"
#include "mem/pmm.h"
#include <limine.h>
#include <system.h>

void kmain(void)
{
	gdt_init();
	idt_init();
	acpi_init();
	keyboard_init();
	fbinit();

	for (;;)
	{
		keyboard_interrupt_handler();
	}
}
