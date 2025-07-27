#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "kbd/keyboard.h"
#include <system.h>

void kmain(void)
{
	gdt_init();
	idt_init();
	keyboard_init();
	fbinit();

	for (;;)
	{
		serial_printf("did we actually start???\n");
	}
}
