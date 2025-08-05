#include "acpi.h"
#include "cpu/cpuid.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "mem/paging.h"
#include "mem/pmm.h"
#include <uacpi/log.h>
#include <uacpi/status.h>

#include <limine.h>
#include <system.h>
#include <uacpi/event.h>
#include <uacpi/uacpi.h>

#include "hpet.h"

void kmain(void) {
	gdt_init();
	idt_init();
	bitmap_init();
	load_pages();
	acpi_init();
	hpet_init();
	hpet_configure_timer(0, 100);

	for (;;) {
	}
}
