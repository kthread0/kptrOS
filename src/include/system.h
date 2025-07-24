#include <stddef.h>
#include <stdint.h>

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void panic(void);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
