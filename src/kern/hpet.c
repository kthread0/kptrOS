#include "acpi.h"
#include "serial/serial.h"
#include "timers.h"
#include <stdint.h>
#include <system.h>
#include <uacpi/acpi.h>
#include <uacpi/tables.h>
#include <uacpi/uacpi.h>

static inline uint64_t hpet_read_register(size_t offset) {
	return *((volatile uint64_t *)(hpet_base + offset));
}

static inline void hpet_write_register(size_t offset, uint64_t value) {
	*((volatile uint64_t *)(hpet_base + offset)) = value;
}

uint64_t hpet_get_counter(void) {
	if (!hpet_base) {
		serial_write("[ ERR ] HPET not initialized\n");
		return 0;
	}
	return hpet_read_register(HPET_MAIN_COUNTER);
}

int hpet_configure_timer(uint64_t timer, uint64_t interval_ms) {
	if (!hpet_base) {
		serial_write("[ ERR ] HPET not initialized\n");
		return -1;
	}

	uint64_t interval_ticks = (1000000000000000 / hpet_period) * interval_ms;

	uint64_t timer_config_offset = HPET_TIMER_CONFIG_CAPABILITY(timer);
	uint64_t timer_comparator_offset = HPET_TIMER_COMPARATOR(timer);

	hpet_write_register(timer_config_offset, HPET_PERIODIC_CNF | HPET_INT_ENABLE_CNF);
	hpet_write_register(timer_comparator_offset, interval_ticks);

	serial_printf("[ INFO ] HPET Timer %d configured for %d ms interval\n", timer, interval_ms);
	return 0;
}

int timer_init(void) {
	uacpi_table hpet_table;
	uacpi_table_find_by_signature("HPET", &hpet_table);

	hpet_base = uacpi_kernel_map(hpet_table.virt_addr, HPET_REG_SIZE);

	uint64_t capabilities = hpet_read_register(HPET_GENERAL_CAPABILITIES_ID);
	hpet_period = (capabilities >> 32) & 0xFFFFFFFF;

	hpet_write_register(HPET_GENERAL_CONFIGURATION, HPET_ENABLE_CNF);
	hpet_configure_timer(0, 100);

	serial_printf("[ OK ] HPET\n");
	return 0;
}
