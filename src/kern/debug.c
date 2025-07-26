#include "../kern/serial/serial.h"
#include <limine.h>
#include <stddef.h>
#include <system.h>

// Memory map request
static volatile struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST, .revision = 0};

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

	uint64_t limine_base_revision[3];
	// Ensure the bootloader actually understands our base revision (see spec).
	if (limine_base_revision[2] != 0)
	{
		serial_printf("Invalid bootloader revision\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	}
	else
	{
		serial_printf("Bootloader revision: %d\n", LIMINE_API_REVISION);
	}

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

	serial_printf("Limine Requests Debug Complete.\n");
}
