#include "../kern/serial/serial.h"
#include <panic.h>
#include <system.h>

// Capture CPU state
void capture_cpu_state(cpu_state_t *state) {
	if (state == NULL) {
		serial_write("[ ERR ] capture_cpu_state called with NULL state\n");
		return;
	}

	asm(
			"lea (%%rip), %0\n"
			"mov %%rsp, %1\n"
			"mov %%rbp, %2\n"
			"pushfq\n"
			"pop %3\n"
			: "=r"(state->rip), "=r"(state->rsp), "=r"(state->rbp), "=r"(state->rflags)
			:
			: "memory");

	serial_printf("[ DEBUG ] Captured CPU State: RIP=%x, RSP=%x, RBP=%x\n",
								state->rip, state->rsp, state->rbp);
}

// Core dump function
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

// Panic handler
void panic(cpu_state_t *state) {
	serial_write("\n*** KERNEL PANIC ***\n");
	serial_write("System halted due to critical error.\n");

	if (state == NULL) {
		serial_write("[ ERR ] Panic called with NULL state. Cannot generate core dump.\n");
		for (;;) {
			asm("hlt");
		}
	}

	// Print core dump
	coredump(state);

	// Halt CPU
	for (;;) {
		asm("hlt");
	}
}
