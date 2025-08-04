#include "limine.h"
#include "mem/pmm.h"
#include "serial/serial.h"
#include "uacpi/platform/types.h"
#include "uacpi/status.h"
#include <panic.h>
#include <stdint.h>
#include <system.h>

#include <uacpi/uacpi.h>

struct limine_rsdp_request rsdp_request = {.id = LIMINE_RSDP_REQUEST, .revision = 0};

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
	free_page(&addr, len);
}

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *rsdp) {
	return rsdp_request.response->address;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
	void *tmp = NULL;
	alloc_page(&tmp, len);
	return tmp;
}

void uacpi_kernel_log(uacpi_log_level log, const uacpi_char *buf) {
	serial_printf("uACPI: %s", buf);
}

int acpi_init(void) {
	void *tmp = NULL;
	alloc_page(&tmp, PAGE_SIZE);
	if (!tmp) {
		serial_printf("Failed to allocate page\n");
		return -1;
	} else {
		serial_printf("tmp: %x, PAGE_SIZE: %x\n", tmp, PAGE_SIZE);
	}

	uacpi_status ret = uacpi_setup_early_table_access(tmp, PAGE_SIZE);
	if (uacpi_unlikely_error(ret)) {
		serial_printf("uacpi_initialize error: %s\n", uacpi_status_to_string(ret));
		return -1;
	}
	return 0;
}
