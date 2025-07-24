#include <stdint.h>

// Example implementation of `outb` for serial communication
static inline void outb(uint16_t port, uint8_t value) {
	__asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Serial port base address (COM1)
#define SERIAL_PORT 0x3F8

void serial_write(const char *str) {
	while (*str) {
		outb(0x3F8,
			 *str++); // Example: Write each character to the COM1 serial port
	}
}

// Write a hexadecimal value to the serial port
void serial_write_hex(uint64_t value) {
	const char *hex_digits = "0123456789ABCDEF";
	char buffer[17]; // 16 hex digits + null terminator
	buffer[16] = '\0';

	for (int i = 15; i >= 0; i--) {
		buffer[i] = hex_digits[value & 0xF];
		value >>= 4;
	}

	serial_write("0x");
	serial_write(buffer);
}

// Write a decimal value to the serial port
void serial_write_dec(uint64_t value) {
	char buffer[21]; // Maximum length for 64-bit decimal + null terminator
	int i = 20;
	buffer[i] = '\0';

	do {
		buffer[--i] = '0' + (value % 10);
		value /= 10;
	} while (value > 0);

	serial_write(&buffer[i]);
}
