#include <uacpi/platform/arch_helpers.h>
#include <uacpi/types.h>

void *acpi_map(uacpi_phys_addr addr, uacpi_size len);
void acpi_unmap(void *addr, uacpi_size len);
