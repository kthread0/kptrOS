#ifndef _ACPI_MALLOC_H_
#define _ACPI_MALLOC_H_

#include <uacpi/types.h>

void *acpi_malloc(uacpi_size size);
void acpi_free(void *ptr);
void *acpi_map(uacpi_phys_addr addr, uacpi_size len);
void acpi_unmap(void *addr, uacpi_size len);

#endif // _ACPI_MALLOC_H_
