#include "limine.h"
#include "mem/pmm.h"
#include "serial/serial.h"
#include "uacpi/log.h"
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
	if (!rsdp_request.response || !rsdp_request.response->address) {
		uacpi_kernel_log(UACPI_LOG_ERROR, "RSDP not found!");
		return UACPI_STATUS_NOT_FOUND;
	}

	*rsdp = rsdp_request.response->address;
	return UACPI_STATUS_OK;
}

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
	len = PAGE_SIZE;

	void *tmp = NULL;
	alloc_page(&tmp, len);

	if (!tmp) {
		serial_printf("[ ERR ] Failed to allocate memory for mapping\n");
		return NULL;
	}

	if (!memcpy(tmp, (void *)addr, len)) {
		serial_printf("[ ERR ] Failed to copy data from physical memory\n");
		free_page(&tmp, len);
		return NULL;
	}

	return tmp;
}

void uacpi_kernel_log(uacpi_log_level log, const uacpi_char *buf) {
	serial_printf("uACPI [%d]: %s", log, buf);
}

int acpi_init(void) {
	uacpi_status ret;
	void *tmp = NULL;

	alloc_page(&tmp, PAGE_SIZE);

	if (!tmp) {
		uacpi_kernel_log(UACPI_LOG_ERROR, "[ ERR ] Failed to allocate memory for uACPI");
		return -1;
	}

	memset(tmp, 0, PAGE_SIZE);

	ret = uacpi_setup_early_table_access(tmp, PAGE_SIZE);

	if (ret != UACPI_STATUS_OK) {
		uacpi_kernel_log(UACPI_LOG_ERROR, uacpi_status_to_string(ret));
		free_page(&tmp, PAGE_SIZE);
		return -1;
	}

	free_page(&tmp, PAGE_SIZE);
	uacpi_kernel_log(UACPI_LOG_INFO, "uACPI Started\n");
	return 0;
}
