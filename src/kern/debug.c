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

volatile struct limine_rsdp_request rsdp_request
	__attribute__((aligned(8))) = {.id = LIMINE_RSDP_REQUEST, .revision = 0};

// Paging mode request for 4-level paging
static volatile struct limine_paging_mode_request paging_mode_request = {
	.id = LIMINE_PAGING_MODE_REQUEST,
	.revision = 0,
	.mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.max_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
	.min_mode = LIMINE_PAGING_MODE_X86_64_4LVL};

__attribute__((
	used, section(".limine_requests"))) static volatile LIMINE_BASE_REVISION(3);

void debug_limine_requests()
{
	serial_printf("Debugging Limine Requests...\n");

	// Ensure the bootloader actually understands our base revision (see spec).
	if (LIMINE_BASE_REVISION_SUPPORTED == false)
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

	serial_printf("Limine Requests Debug Complete.\n");
}
