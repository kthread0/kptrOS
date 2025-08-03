#include <limine.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define KERNEL_CS 0x08
#define IDT_MAX_DESCRIPTORS 256
#define PAGE_SIZE 4096

int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void debug_limine_requests();
void debug();
void fbinit();
