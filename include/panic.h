#include <stdint.h>

typedef struct {
	uint64_t rip; // Instruction pointer
	uint64_t rsp; // Stack pointer
	uint64_t rbp; // Base pointer
	uint64_t rflags;
	uint64_t registers[16]; // General-purpose registers
} cpu_state_t;

void panic(cpu_state_t *);
void capture_cpu_state(cpu_state_t *);
