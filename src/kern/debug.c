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
	serial_write("Debugging Limine Requests...\n");

	// Kernel address request
	if (executable_address_request.response == NULL)
	{
		serial_write("Kernel address request failed: response is NULL.\n");
	}
	else
	{
		serial_write("Kernel address request succeeded.\n");
	}

	// SMBIOS request
	if (smbios_request.response == NULL)
	{
		serial_write("SMBIOS request failed: response is NULL.\n");
	}
	else
	{
		serial_write("SMBIOS request succeeded.\n");
	}

	serial_write("Limine Requests Debug Complete.\n");
}
