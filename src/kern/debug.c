#include "../kern/serial/serial.h"

#include <limine.h>
#include <stddef.h>
#include <system.h>

// Entry point request
static volatile struct limine_executable_address_request executable_address_request
								__attribute__((aligned(8))) = { .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST, .revision = 0 };

// SMBIOS request
static volatile struct limine_smbios_request smbios_request
								__attribute__((aligned(8))) = { .id = LIMINE_SMBIOS_REQUEST, .revision = 0 };

// Paging mode request for 4-level paging
static volatile struct limine_paging_mode_request paging_mode_request = { .id = LIMINE_PAGING_MODE_REQUEST,
																																					.revision = 0,
																																					.mode = LIMINE_PAGING_MODE_X86_64_4LVL,
																																					.max_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
																																					.min_mode = LIMINE_PAGING_MODE_X86_64_4LVL };

// Firmware type request
static volatile struct limine_firmware_type_request firmware_type_request = { .id = LIMINE_FIRMWARE_TYPE_REQUEST, .revision = 0 };

// Stack size request
static volatile struct limine_stack_size_request stack_size_request = { .id = LIMINE_STACK_SIZE_REQUEST, .revision = 0 };

void debug_limine_requests() {
	// Kernel address request
	if (executable_address_request.response == NULL) {
		serial_printf("Kernel address request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	} else {
		serial_printf("Kernel address: %x\n", executable_address_request.response->virtual_base);
	}

	// SMBIOS request
	if (smbios_request.response == NULL) {
		serial_printf("SMBIOS request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	} else {
		serial_printf("SMBIOS: %x\n", smbios_request.response->entry_64);
	}

	// Paging mode request
	if (paging_mode_request.response == NULL) {
		serial_printf("Paging mode request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	} else {
		serial_printf("Paging mode: %d\n", paging_mode_request.response->mode);
	}

	if (firmware_type_request.response == NULL) {
		serial_printf("Firmware type request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	} else {
		serial_printf("Firmware type: %d\n", firmware_type_request.response->firmware_type);
	}

	if (stack_size_request.response == NULL) {
		serial_printf("Stack size request failed: response is NULL.\n");
		cpu_state_t state;
		capture_cpu_state(&state);
		panic(&state);
	} else {
		serial_printf("Stack size: %x\n", stack_size_request.stack_size);
	}
}
