#include "../include/limine.h"
#include "../kern/serial/serial.h"
#include <stddef.h>

// Entry point request
volatile struct limine_executable_address_request executable_address_request
	__attribute__((aligned(8))) = {.id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
								   .revision = 0};

// SMBIOS request
volatile struct limine_smbios_request smbios_request
	__attribute__((aligned(8))) = {.id = LIMINE_SMBIOS_REQUEST, .revision = 0};

void debug_limine_requests()
{
	serial_printf("Debugging Limine Requests...\n");

	// Kernel address request
	if (executable_address_request.response == NULL)
	{
		serial_printf("Kernel address request failed: response is NULL.\n");
	}
	else
	{
		serial_printf("Kernel address: %x\n",
					  executable_address_request.response->virtual_base);
	}

	// SMBIOS request
	if (smbios_request.response == NULL)
	{
		serial_printf("SMBIOS request failed: response is NULL.\n");
	}
	else
	{
		serial_printf("SMBIOS: %s\n", smbios_request.response->entry_32);
	}

	serial_printf("Limine Requests Debug Complete.\n");
}
