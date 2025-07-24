#include "../kern/serial/serial.h"
#include "stdio.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
		uint64_t rip; // Instruction pointer
		uint64_t rsp; // Stack pointer
		uint64_t rbp; // Base pointer
		uint64_t rflags;
		uint64_t registers[16]; // General-purpose registers
} cpu_state_t;

void enable_cursor(uint8_t, uint8_t);
void panic(cpu_state_t *);
void capture_cpu_state(cpu_state_t *);
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
void debug_limine_requests();
