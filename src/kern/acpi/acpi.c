#include "kern/cpu/access.h"
#include "kern/mem/pmm.h"
#include "uacpi/platform/types.h"
#include "uacpi/status.h"
#include <limine.h>
#include <system.h>
#include <uacpi/kernel_api.h>
#include <uacpi/types.h>

// Memory allocation

static volatile struct limine_rsdp_request rsdp_request
	__attribute__((aligned(8))) = {.id = LIMINE_RSDP_REQUEST, .revision = 0};

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len)
{
	return pmm_alloc(len);
}
void uacpi_kernel_unmap(void *addr, uacpi_size len)
{
	pmm_free_page(&addr);
}
void *uacpi_kernel_alloc(uacpi_phys_addr addr)
{
	return pmm_alloc(addr);
}
void uacpi_kernel_free(void *addr)
{
	pmm_free_page(&addr);
}
void uacpi_kernel_log(uacpi_log_level level, const char *fmt)
{
	va_list args;
	serial_printf(fmt);
	va_end(args);
}

uint8_t uacpi_platform_inb(uint16_t port)
{
	return inb(port);
}
void uacpi_platform_outb(uint16_t port, uint8_t value)
{
	outb(port, value);
}

uint16_t uacpi_platform_inw(uint16_t port)
{
	return inb(port);
}
void uacpi_platform_outw(uint16_t port, uint16_t value)
{
	outb(port, value);
}

uint32_t uacpi_platform_inl(uint16_t port)
{
	return inb(port);
}
void uacpi_platform_outl(uint16_t port, uint32_t value)
{
	outb(port, value);
}

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address)
{
	*out_rsdp_address = rsdp_request.response->address;
	return UACPI_STATUS_OK;
}
