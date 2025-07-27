#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// Function declarations
void serial_write(const char *str);
void serial_write_hex(uint64_t value);
void serial_write_dec(uint64_t value);
void serial_printf(const char *format, ...);
void int_to_string(int value, char *buffer);

#endif // SERIAL_H
