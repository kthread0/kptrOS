#include "../include/system.h"
#include "../kern/serial/serial.h"

// Function to capture CPU state
void capture_cpu_state(cpu_state_t *state) {
	__asm__ volatile(
			"lea (%%rip), %0\n" // Load the address of the next instruction into RIP
			"mov %%rsp, %1\n"		// Move stack pointer into RSP
			"mov %%rbp, %2\n"		// Move base pointer into RBP
			"pushfq\n"					// Push flags register onto the stack
			"pop %3\n"					// Pop flags register into RFLAGS
			: "=r"(state->rip), "=r"(state->rsp), "=r"(state->rbp), "=r"(state->rflags)
			:
			: "memory");
}

void coredump(cpu_state_t *state) {
	serial_write("\n--- CORE DUMP ---\n");

	serial_write("RIP: ");
	serial_write_hex(state->rip);
	serial_write("\n");

	serial_write("RSP: ");
	serial_write_hex(state->rsp);
	serial_write("\n");

	serial_write("RBP: ");
	serial_write_hex(state->rbp);
	serial_write("\n");

	serial_write("RFLAGS: ");
	serial_write_hex(state->rflags);
	serial_write("\n");

	for (int i = 0; i < 16; i++) {
		serial_write("REG[");
		serial_write_dec(i);
		serial_write("]: ");
		serial_write_hex(state->registers[i]);
		serial_write("\n");
	}

	serial_write("--- END CORE DUMP ---\n");
}

void panic(cpu_state_t *state) {
	serial_write("\n*** KERNEL PANIC ***\n");
	serial_write("System halted due to critical error.\n");

	// Print coredump information
	coredump(state);

	// Halt the CPU
	for (;;) {
		asm("hlt");
	}
}
