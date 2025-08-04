#include <uacpi/uacpi.h>

extern uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *rsdp_request);
extern void uacpi_kernel_log(uacpi_log_level, const uacpi_char *);
extern void *uacpi_kernel_map(uacpi_virt_addr addr, uacpi_size len);
extern void uacpi_kernel_unmap(void *addr, uacpi_size len);
int acpi_init(void);
