#include "../kern/serial/serial.h"
#include "stdio.h"
#include <stddef.h>
#include <stdint.h>

void enable_cursor(uint8_t, uint8_t);
void panic(void);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
