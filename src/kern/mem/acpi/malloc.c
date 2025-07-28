#include "../pmm.h"

#include <system.h>
#include <uacpi/platform/arch_helpers.h>
#include <uacpi/types.h>

void *acpi_malloc(uacpi_phys_addr *addr, uacpi_size size) {
	pmm_alloc(&addr, size);
}

void acpi_free(void *ptr, size_t len) {
	if (ptr) {
		pmm_free(&ptr, len);
	}
}

void *acpi_map(uacpi_phys_addr addr, uacpi_size len) {
	return pmm_alloc(&addr, len);
}

void acpi_unmap(void *addr, size_t len) {
	pmm_free(&addr, len);
}
