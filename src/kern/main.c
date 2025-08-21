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

#include "timers.h"

void kinit(void) {
	gdt_init();
	idt_init();
	bitmap_init();
	acpi_init();
	timer_init();
}

void kmain(void) {
	kinit();

	for (;;) {
	}
}
