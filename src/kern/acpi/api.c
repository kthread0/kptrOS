#include "kern/cpu/access.h"
#include "kern/mem/acpi/malloc.h"
#include "uacpi/platform/types.h"
#include "uacpi/status.h"

#include <limine.h>
#include <uacpi/kernel_api.h>
#include <uacpi/status.h>
#include <uacpi/types.h>

static volatile struct limine_rsdp_request rsdp_request = { .id = LIMINE_RSDP_REQUEST, .revision = 0 };

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
	return acpi_map(addr, len);
}

void uacpi_kernel_unmap(void *addr, uacpi_size len) {
	acpi_unmap(addr, len);
}

void *uacpi_kernel_alloc(uacpi_size size) {
	return acpi_malloc(size); // Allocate memory
}

void uacpi_kernel_free(void *mem) {
	if (mem) {
		acpi_free(mem); // Free allocated memory
	}
}

void uacpi_kernel_log(uacpi_log_level, const uacpi_char *) {
}
