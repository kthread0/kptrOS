#include "debounce.h"
#include "../cpu/clk/timer.h"

// Last recorded timestamp for each key
static uint64_t key_last_press_time[MAX_KEYS];

void keyboard_debounce_init(void) {
	for (int i = 0; i < MAX_KEYS; i++) {
		key_last_press_time[i] = 0;
	}
}

bool keyboard_debounce_check(uint8_t scancode, uint64_t current_time) {
	if (scancode >= MAX_KEYS) {
		return true;
	}

	uint64_t last_time = key_last_press_time[scancode];

	key_last_press_time[scancode] = sleep(DEBOUNCE_DELAY_MS);

	if (last_time == 0 || (current_time - last_time) > DEBOUNCE_DELAY_MS) {
		return true;
	}

	return false;
}
