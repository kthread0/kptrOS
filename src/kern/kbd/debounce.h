#ifndef KEYBOARD_DEBOUNCE_H
#define KEYBOARD_DEBOUNCE_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_KEYS 128
#define DEBOUNCE_DELAY_MS 25

void keyboard_debounce_init (void);
bool keyboard_debounce_check (uint8_t scancode, uint64_t current_time);

#endif /* KEYBOARD_DEBOUNCE_H */
