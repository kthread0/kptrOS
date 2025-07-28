#include "kern/cpu/access.h"
#include "kern/mem/acpi/malloc.h"
#include "uacpi/platform/types.h"
#include "uacpi/status.h"

#include <system.h>
#include <uacpi/kernel_api.h>
#include <uacpi/status.h>
#include <uacpi/types.h>

static volatile struct limine_rsdp_request rsdp_request = { .id = LIMINE_RSDP_REQUEST, .revision = 0 };

void *uacpi_kernel_map(uacpi_phys_addr addr, uacpi_size len) {
	return acpi_alloc(addr, len);
}
void uacpi_kernel_unmap(void *addr, uacpi_size len) {
	return acpi_free(&addr, len);
}
void *uacpi_kernel_alloc(uacpi_phys_addr addr) {
	return acpi_alloc(addr, len);
}
void uacpi_kernel_free(uacpi_status *addr) {
	return acpi_free_page(&addr);
}
void uacpi_kernel_log(uacpi_log_level level, const char *fmt) {
	va_list args;
	serial_printf(fmt);
	va_end(args);
}

uint8_t uacpi_platform_inb(uint16_t port) {
	return inb(port);
}
void uacpi_platform_outb(uint16_t port, uint8_t value) {
	return outb(port, value);
}

uint16_t uacpi_platform_inw(uint16_t port) {
	return inb(port);
}
uacpi_status uacpi_platform_outw(uint16_t port, uint16_t value) {
	return outb(port, value);
}

uint32_t uacpi_platform_inl(uint16_t port) {
	return inb(port);
}
uacpi_status uacpi_platform_outl(uint16_t port, uint32_t value) {
	return outb(port, value);
}

uacpi_status uacpi_kernel_get_rsdp(uacpi_phys_addr *out_rsdp_address) {
	return *out_rsdp_address = rsdp_request.response->address;
}

uacpi_status uacpi_kernel_create_mutex() {
	serial_printf("uacpi_kernel_create_mutex called\n");
	// Implement kernel mutex creation logic
}

uacpi_status uacpi_kernel_free_mutex() {
	serial_printf("uacpi_kernel_free_mutex called\n");
	// Implement kernel mutex free logic
}

uacpi_status uacpi_kernel_create_event() {
	serial_printf("uacpi_kernel_create_event called\n");
	// Implement kernel event creation logic
}

uacpi_status uacpi_kernel_free_event() {
	serial_printf("uacpi_kernel_free_event called\n");
	// Implement kernel event free logic
}

uacpi_status uacpi_kernel_stall() {
	serial_printf("uacpi_kernel_stall called\n");
	// Implement kernel stall logic
}

uacpi_status uacpi_kernel_initialize() {
	serial_printf("uacpi_kernel_initialize called\n");
	// Implement kernel initialization logic
}

uacpi_status uacpi_kernel_deinitialize() {
	serial_printf("uacpi_kernel_deinitialize called\n");
	// Implement kernel deinitialization logic
}
