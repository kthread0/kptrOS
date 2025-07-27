#include <stdarg.h>
#include <stdint.h>

// Example implementation of `outb` for serial communication
static inline void
outb (uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Serial port base address (COM1)
#define SERIAL_PORT 0x3F8

void
serial_write (const char *str)
{
	while (*str)
		{
			outb (SERIAL_PORT,
						*str++); // Write each character to the COM1 serial port
		}
}

// Write a hexadecimal value to the serial port
void
serial_write_hex (uint64_t value)
{
	const char *hex_digits = "0123456789ABCDEF";
	char buffer[17]; // 16 hex digits + null terminator
	buffer[16] = '\0';

	for (int i = 15; i >= 0; i--)
		{
			buffer[i] = hex_digits[value & 0xF];
			value >>= 4;
		}

	serial_write ("0x");
	serial_write (buffer);
}

// Write a decimal value to the serial port
void
serial_write_dec (uint64_t value)
{
	char buffer[21]; // Maximum length for 64-bit decimal + null
									 // terminator
	int i = 20;
	buffer[i] = '\0';

	do
		{
			buffer[--i] = '0' + (value % 10);
			value /= 10;
		}
	while (value > 0);

	serial_write (&buffer[i]);
}
// Convert an integer to a string (decimal)
void
int_to_string (int value, char *buffer)
{
	int i = 0;
	int is_negative = 0;

	// Handle negative numbers
	if (value < 0)
		{
			is_negative = 1;
			value = -value;
		}

	// Convert the number to a string
	do
		{
			buffer[i++] = '0' + (value % 10);
			value /= 10;
		}
	while (value > 0);

	// Add the negative sign if needed
	if (is_negative)
		{
			buffer[i++] = '-';
		}

	// Null-terminate the string
	buffer[i] = '\0';

	// Reverse the string
	for (int j = 0; j < i / 2; j++)
		{
			char temp = buffer[j];
			buffer[j] = buffer[i - j - 1];
			buffer[i - j - 1] = temp;
		}
}

// Minimal printf-like function for serial output
void
serial_printf (const char *format, ...)
{
	va_list args;
	va_start (args, format);

	while (*format)
		{
			if (*format == '%')
				{
					format++; // Skip '%'

					switch (*format)
						{
						case 'd':
							{ // Decimal
								int value = va_arg (args, int);
								char buffer[21];
								int_to_string (value, buffer);
								serial_write (buffer);
								break;
							}
						case 'x':
							{ // Hexadecimal
								uint64_t value = va_arg (args, uint64_t);
								serial_write_hex (value);
								break;
							}
						case 's':
							{ // String
								const char *str = va_arg (args, const char *);
								serial_write (str);
								break;
							}
						case '%':
							{ // Literal '%'
								serial_write ("%");
								break;
							}
						default:
							{ // Unknown format specifier
								serial_write ("[ERR:UNKNOWN]");
								break;
							}
						}
				}
			else
				{
					// Write regular characters
					outb (SERIAL_PORT, *format);
				}

			format++;
		}

	va_end (args);
}
