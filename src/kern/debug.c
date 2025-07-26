#include "../kern/serial/serial.h"
#include <limine.h>
#include <stddef.h>
#include <system.h>

void debug_limine_requests()
{
	if (memmap_request.response == NULL)
	{
		serial_printf("Memory map request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
	else
	{
		serial_printf("Memory map request: %x\n",
					  memmap_request.response->entries);
	}

	// Kernel address request
	if (executable_address_request.response == NULL)
	{
		serial_printf("Kernel address request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
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
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
	else
	{
		serial_printf("SMBIOS: %x\n", smbios_request.response->entry_64);
	}

	// Paging mode request
	if (paging_mode_request.response == NULL)
	{
		serial_printf("Paging mode request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
	else
	{
		serial_printf("Paging mode: %d\n", paging_mode_request.response->mode);
	}
}
