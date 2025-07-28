#include <system.h>
#include <uacpi/kernel_api.h>
#include <uacpi/status.h>
#include <uacpi/types.h>

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len);
void uacpi_kernel_unmap(void *addr, uacpi_size len);
void *uacpi_kernel_alloc(uacpi_phys_addr addr);
void uacpi_kernel_free(uacpi_status *addr);
void uacpi_kernel_log(uacpi_log_level level, const char *fmt);
uint8_t uacpi_platform_inb(uint16_t port);
void uacpi_platform_outb(uint16_t port, uint8_t value);
uint16_t uacpi_platform_inw(uint16_t port);
uacpi_status uacpi_platform_outw(uint16_t port, uint16_t value);
uint32_t uacpi_platform_inl(uint16_t port);
uacpi_status uacpi_platform_outl(uint16_t port, uint32_t value);
uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address);
uacpi_status uacpi_kernel_create_mutex();
uacpi_status uacpi_kernel_free_mutex();
uacpi_status uacpi_kernel_create_event();
uacpi_status uacpi_kernel_free_event();
uacpi_status uacpi_kernel_stall();
uacpi_status uacpi_kernel_initialize();
uacpi_status uacpi_kernel_deinitialize();
