#include "../cpu/access.h"
#include "keyboard.h"
#include <system.h>

void keyboard_test_controller(void)
{
	serial_write("Testing keyboard controller...\n");

	// Check if keyboard controller is ready
	uint8_t status = inb(0x64);
	serial_printf("Keyboard status: %x\n", status);

	// Check if there's data in the buffer
	if (status & 0x01)
	{
		uint8_t data = inb(0x60);
		serial_printf("Data in keyboard buffer: %x\n", data);
	}

	// Test self-test command
	outb(0x64, 0xAA); // Self-test command
	io_wait();

	// Wait for response
	while (!(inb(0x64) & 0x01))
		;
	uint8_t result = inb(0x60);
	serial_printf("Self-test result: %x (should be 0x55)\n", result);
}
